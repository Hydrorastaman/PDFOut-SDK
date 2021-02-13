#pragma once

#include <vector>
#include <FontCollection.h>

namespace pdfout{ namespace impl{

  class FontCollectionImpl : public FontCollection{ 
  public:
    explicit FontCollectionImpl(std::vector<Font *> const &collection);
    ~FontCollectionImpl(void) {}

    std::size_t getCount(void) const;
    Font *getFont(std::size_t index) const;

  private:
    FontCollectionImpl(FontCollectionImpl const &) = delete;
    FontCollectionImpl &operator=(FontCollectionImpl const &) = delete;

  private:
    std::vector<Font *> mCollection;
  };

}}
