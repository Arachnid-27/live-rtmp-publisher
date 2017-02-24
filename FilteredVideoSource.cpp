#include "FilteredVideoSource.h"

char* FilteredVideoSource::getNextFrame() {
    char *result = mSource.getNextFrame();

    if (result == NULL) {
        return NULL;
    }

    for (auto filter : mFilters) {
        result = filter->filter(reinterpret_cast<const unsigned char *>(result), mSource.getWidth(), mSource.getHeight());
        if (result == NULL) {
            return NULL;
        }
    }

    return result;
}
