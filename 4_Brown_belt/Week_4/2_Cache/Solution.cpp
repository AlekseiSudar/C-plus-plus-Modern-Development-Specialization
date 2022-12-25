#include "Common.h"
#include <mutex>
#include <map>
#include <iterator>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const Settings& settings
    ) : books_unpacker_(books_unpacker)
      , max_memory(settings.max_memory)
      , free_memory(max_memory) {
    }

    BookPtr GetBook(const string& book_name) override {
        // If cache has book
        {
            lock_guard lock(m);
            for (auto it = cache_.begin(), it_end = cache_.end(); it != it_end; it++){
                if (it->second->GetName() == book_name){
                    if (it != --it_end){
                        cache_[it_end->first + 1] = it->second;
                        cache_.erase(it);
                    }
                    return cache_.rbegin()->second;
                }
            }
        }

        //else
        BookPtr book = books_unpacker_->UnpackBook(book_name);
        size_t book_size = book->GetContent().size();

        if (max_memory < book_size) return book;
        
        {
            lock_guard a(m);

            while (free_memory < book_size) {
                free_memory += cache_.begin()->second->GetContent().size();
                cache_.erase(cache_.begin());
            }

            if (cache_.empty())
                cache_[0] = book;
            else
                cache_[cache_.rbegin()->first + 1] = book;
            
            free_memory -= book_size;
        }
        
        return book;
    }
private:
    shared_ptr<IBooksUnpacker> books_unpacker_;
    mutex m;

    size_t max_memory;
    size_t free_memory;
    map<int, BookPtr> cache_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
    return make_unique<LruCache>(move(books_unpacker), settings);
}
