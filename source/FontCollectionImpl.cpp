#include <FontCollectionImpl.h>

namespace pdfout{ namespace impl{

  FontCollectionImpl::FontCollectionImpl(std::vector<Font *> const &collection)
    : FontCollection(),
    mCollection(collection){
  }

  std::size_t FontCollectionImpl::getCount(void) const{
    return mCollection.size();
  }

  Font *FontCollectionImpl::getFont(std::size_t index) const{
    return mCollection[index];
  }

}}
