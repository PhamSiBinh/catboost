
#include "pool.h"

TMemoryPool::IGrowPolicy* TMemoryPool::TLinearGrow::Instance() noexcept {
    return SingletonWithPriority<TLinearGrow, 0>();
}

TMemoryPool::IGrowPolicy* TMemoryPool::TExpGrow::Instance() noexcept {
    return SingletonWithPriority<TExpGrow, 0>();
}

void TMemoryPool::AddChunk(size_t hint) {
    const size_t dataLen = Max(BlockSize_, hint);
    TBlock nb = Alloc_->Allocate(FastClp2(dataLen + sizeof(TChunk)));

    BlockSize_ = GrowPolicy_->Next(dataLen);
    Current_ = new (nb.Data) TChunk(nb.Len - sizeof(TChunk));
    Chunks_.PushBack(Current_);
}

void TMemoryPool::DoClear(bool keepfirst) noexcept {
    while (!Chunks_.Empty()) {
        TChunk* c = Chunks_.PopBack();

        if (keepfirst && Chunks_.Empty()) {
            c->ResetChunk();
            Chunks_.PushBack(c);
            Current_ = c;
            BlockSize_ = c->BlockLength() - sizeof(TChunk);
            return;
        }

        TBlock b = {c, c->BlockLength()};

        c->~TChunk();
        Alloc_->Release(b);
    }

    Current_ = &Empty_;
    BlockSize_ = Origin_;
}
