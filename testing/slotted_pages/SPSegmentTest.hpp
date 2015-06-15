//
// Created by Martin on 15.06.2015.
//

#ifndef PROJECT_SPSEGMENTTEST_HPP
#define PROJECT_SPSEGMENTTEST_HPP

class SPSegmentTest : public Test {
private:
  BufferManager *bufferManager;
  std::vector<Record *> createdRecords;
  std::vector<char *> allocatedData;
public:
  static const uint64_t SEGMENT_ID = 1;
  SPSegment *segment;

  void SetUp() {
    bufferManager = new BufferManager(65536);
    segment = new SPSegment(*bufferManager, SEGMENT_ID);
  }

  void TearDown() {
    delete this->segment;
    delete this->bufferManager;

    std::string filename = std::to_string(SEGMENT_ID);
    remove(filename.c_str());

    for (auto it : createdRecords) {
      delete it;
    }
    for (auto it : allocatedData) {
      free(it);
    }
  };

  /**
   * Allocates data that will automatically be free'd at the end of the test.
   */
  char *allocateData(size_t len) {
    char *data = (char *) malloc(len);
    this->allocatedData.push_back(data);
    return data;
  }

  Record *newRecord(size_t len) {
    char *data = allocateData(len);
    return new Record(len, data);
  }

  void testUpdate(size_t initialLength, size_t updatedLength) {
    TID tid = insertRecord(initialLength);

    char *updatedData = allocateData(updatedLength);
    const Record updateRecord(updatedLength, updatedData);
    segment->update(tid, updateRecord);
    Record lookedupRecord = segment->lookup(tid);

    expectRecordsEqual(&updateRecord, &lookedupRecord);
  }

  TID insertRecord(size_t length) {
    char *data = allocateData(length);
    const Record record(length, data);
    TID tid = segment->insert(record);
    return tid;
  }

  void expectRecordsEqual(const Record *r1, const Record *r2) {
    EXPECT_EQ(r1->getLen(), r2->getLen());
    const char *d1 = r1->getData();
    const char *d2 = r2->getData();
    bool recordsEqual = std::memcmp(d1, d2, r1->getLen()) == 0;
    EXPECT_TRUE(recordsEqual);
  }

  ::testing::AssertionResult RecordsMatch(const Record &expected,
                                          const Record &actual) {
    unsigned int expectedLen = expected.getLen();
    unsigned int actualLen = actual.getLen();
    if (expectedLen != actualLen) {
      return ::testing::AssertionFailure()
             << "length " << actualLen
             << " != expected " << expectedLen;
    }
    const char *expectedData = expected.getData();
    const char *actualData = actual.getData();
    bool recordsEqual = std::memcmp(expectedData, actualData, expectedLen) == 0;
    if (!recordsEqual) {
      std::string expectedDataStr = SPSegmentTest::printData(expectedData, expectedLen);
      std::string actualDataStr = SPSegmentTest::printData(actualData, actualLen);
      return ::testing::AssertionFailure()
             << "data " << actualDataStr
             << "!= expected " << expectedDataStr;
    }

    return ::testing::AssertionSuccess();
  }

  static std::string printData(const char *data, const unsigned int len) {
    std::string result;
    for (unsigned int i = 0; i < len; i++) {
      result.append(std::to_string(data[i])).append(" ");
    }
    return result;
  }
};

#endif //PROJECT_SPSEGMENTTEST_HPP
