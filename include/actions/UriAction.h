#pragma once

#ifdef __NOT_IMPLEMENTED___

#include <string>

#include <actions/Action.h>

/**
 * @brief A uniform resource identifier (URI) is a string that identifies (resolves to) a resource on the Internet—typically a file that is the destination of a hypertext link, although it can also resolve to a query or other entity.
 */
class UriAction : public Action{
public:
  UriAction(std::string const &uri);
  ~UriAction(void);

  void serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const;

private:
  UriAction(UriAction const &) = delete;
  UriAction &operator=(UriAction const &) = delete;

private:
  std::string mUri;
};
#endif
