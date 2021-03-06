#ifndef _RID_H_
#define _RID_H_

struct RID {
    // pageId 为记录所在页数， slotId 为记录所在槽数（从 0 开始计数）
    int pageId, slotId;

    bool operator==(const RID &b) const {
        return pageId == b.pageId && slotId == b.slotId;
    }

    bool operator<(const RID &b) const {
        return pageId < b.pageId || (pageId == b.pageId && pageId < b.pageId);
    }

    bool operator>(const RID &b) const {
        return pageId > b.pageId || (pageId == b.pageId && pageId > b.pageId);
    }

    bool operator<=(const RID &b) const {
        return pageId < b.pageId || (pageId == b.pageId && pageId <= b.pageId);
    }

    bool operator>=(const RID &b) const {
        return pageId > b.pageId || (pageId == b.pageId && pageId >= b.pageId);
    }

    RID(int inPageId = -1, int inSlotId = -1) {
        pageId = inPageId;
        slotId = inSlotId;
    }
};

#endif