#ifdef __NOT_IMPLEMENTED___
#include <actions/UriAction.h>

UriAction::UriAction(std::string const &uri)
  : mUri(uri){
}

UriAction::~UriAction(void){}

void UriAction::serialize(pdfout::OutputStream *stream, kernel::SerializeParams *params) const{
}
#endif
