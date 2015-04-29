#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

class BufferFrame{

    enum class State {dirty, clean};
public:
    uint64_t pageId;
    unsigned int reader;
    BufferFrame(uint64_t pageId, int fd);
    ~BufferFrame();

    void* getData();
    void setDirty();
    void lock(bool exclusive);
    void unlock();
    void increaseReader();
    void decreaseReader();

    State state;

private:
    void * data;
    int fd;

};

#endif //PROJECT_BUFFER_FRAME_H
