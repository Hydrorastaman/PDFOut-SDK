#pragma once

#include <string>
#include <deque>
#include <mutex>
#include <map>
#include <memory>

#include <Windows.h>

/*! @file */

namespace vpd_internal{
  /**
   * @addtogroup Internal
   * @{
   */

  namespace cpp11{

    class PerformanceLogger{
    private:
      enum PerformanceStatus{
        PerformanceStatusStart = 0,
        PerformanceStatusStop
      };

      struct PerformanceValue{
        std::string mCounterName;
        PerformanceStatus mStatus;
        __int64 mTimestamp;
      };

    public:
      PerformanceLogger(void)
        : mValues(),
        mAccessMutex(){
      }

      ~PerformanceLogger(void) throw() {}

      static PerformanceLogger *getLogger(void){
        std::call_once(onceFlag, [] {performanceLogger.reset(new PerformanceLogger());});

        return performanceLogger.get();
      }

      void startCounter(std::string const &counterName){
        LARGE_INTEGER time0 = {0};
        QueryPerformanceCounter(&time0);
        PerformanceValue value = {counterName, PerformanceStatusStart, time0.QuadPart};
        std::lock_guard<std::mutex> lock(mAccessMutex);
        mValues.push_back(std::move(value));
      }

      void stopCounter(std::string const &counterName){
        LARGE_INTEGER time0 = {0};
        QueryPerformanceCounter(&time0);
        PerformanceValue value = {std::move(counterName), PerformanceStatusStop, time0.QuadPart};
        std::lock_guard<std::mutex> lock(mAccessMutex);
        mValues.push_back(std::move(value));
      }

      void flushToFile(std::wstring const &fileName) const{
        struct Total{
          __int64 mTotalTime;
          int mCount;
        };

        std::map<std::string, Total> total;

        std::lock_guard<std::mutex> lock(mAccessMutex);

        LARGE_INTEGER li = {0};
        QueryPerformanceFrequency(&li);

        double freq = (double) li.QuadPart;

        for (auto const &elem : mValues){
          switch (elem.mStatus){
            case PerformanceStatusStart:
              total[elem.mCounterName].mTotalTime -= elem.mTimestamp;
              ++total[elem.mCounterName].mCount;
              break;

            case PerformanceStatusStop:
              total[elem.mCounterName].mTotalTime += elem.mTimestamp;
              break;
          }
        }

        FILE *file = nullptr;
        if (_wfopen_s(&file, fileName.c_str(), L"w") != 0)
          return;

        fprintf(file, "%-15s | %-5s | %-13s | %-13s\n", "      Name", "Count", "Total T(sec)", "Average T(sec)");
        fprintf(file, "---------------------------------------------------------------\n");
        for (auto const &elem : total)
          fprintf(file, "%-15s | %-5i | %-13.3f | %-10.3f\n", elem.first.c_str(), elem.second.mCount, (double) elem.second.mTotalTime / freq, (double) elem.second.mTotalTime / (freq * elem.second.mCount));

        fclose(file);
      }

      void flushCountersQueue(std::wstring const &fileName) const{
        std::lock_guard<std::mutex> lock(mAccessMutex);

        LARGE_INTEGER li = {0};
        QueryPerformanceFrequency(&li);

        double freq = (double) li.QuadPart;

        FILE *file = _wfopen(fileName.c_str(), L"w");
        if (!file)
          return;

        fprintf(file, "%-15s | %-13s\n", "      Name", "Timestamp (sec)");
        fprintf(file, "---------------------------------------------------------------\n");

        for (auto const &elem : mValues)
          fprintf(file, "%-15s | %-10.3f\n", elem.mCounterName.c_str(), (double) elem.mTimestamp / freq);

        fclose(file);
      }

    private:
      PerformanceLogger(PerformanceLogger const &) = delete;
      PerformanceLogger &operator=(PerformanceLogger const &) = delete;

    private:
      std::deque<PerformanceValue> mValues;
      mutable std::mutex mAccessMutex;
      static std::unique_ptr<PerformanceLogger> performanceLogger;
      static std::once_flag onceFlag;
    };

  } // namespace cpp11

  /*! @} */
} // namespace vpd_internal
