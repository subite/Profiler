#ifndef PROFILER_H
#define PROFILER_H

#include <array>
#include <chrono>
#include <assert.h>
#include <iterator>

namespace Profiler
{

class PTimelineBase {
public:
  struct Index {
    Index() : m_index(Invalid().m_index) {}
    Index(unsigned i) : m_index(i) {}
    bool isValid() { return m_index == Invalid().m_index; }
    static Index Invalid() { return Index{(unsigned)-1};}

    unsigned m_index;
  };
  virtual Index startTimePoint(const char *id) = 0;
  virtual void endTimePoint(Index index) = 0;

protected:
  PTimelineBase() {};
  virtual ~PTimelineBase() {};
  friend class PTimePoint;

  static thread_local PTimelineBase *m_instance;
};

template<class config, std::size_t N>
class PTimeline : private PTimelineBase
{
public:
  typedef typename config::duration Duration;
  typedef decltype(config::clock::now()) TimePoint;

  PTimeline(const char *id)
    : m_id(id)
    , m_indexesUsed(0)
  {
    start();
  }

  void start() {
    m_begin = std::chrono::system_clock::now();
    m_last = m_begin;
    m_instance = this;
  }

  void stop() {
    auto now = std::chrono::system_clock::now();
    m_last = now;
    m_end = now;
    m_instance = nullptr;
  }

  TimePoint getStart() const { return m_begin; }
  TimePoint getEnd() const { return m_end; }

  PTimelineBase::Index startTimePoint(const char *id) override {
    if (m_indexesUsed >= N)
      return PTimelineBase::Index::Invalid();
    auto now = std::chrono::system_clock::now();
    m_points[m_indexesUsed].m_id = id;
    m_points[m_indexesUsed].m_begin = std::chrono::duration_cast<Duration>(now - m_begin);
    return Index{m_indexesUsed++};
  }

  void endTimePoint(PTimelineBase::Index index) override {
    if (index.m_index >= N)
      return;
    auto now = std::chrono::system_clock::now();
    Point &elem = m_points[index.m_index];
    elem.m_end = std::chrono::duration_cast<Duration>((now - m_begin) - elem.m_begin);
  }

  struct Node {
    TimePoint begin;
    Duration duration;
    const char *id;
  };

  class const_iterator
  {
  public:
    typedef const_iterator self_type;
    typedef Node value_type;
    typedef Node& reference;
    typedef Node* pointer;
    typedef int difference_type;
    typedef std::input_iterator_tag iterator_category;
    explicit const_iterator(const PTimeline<config, N> &parent, unsigned index)
      : m_parent(parent)
      , m_index(index)
    {}
    self_type operator++() {
      self_type i = *this;
      m_index++;
      return i;
    }
    self_type operator++(int) {
      m_index++;
      return *this;
    }
    const reference operator*() {
      fillNode();
      return m_node;
    }
    pointer operator->() {
      fillNode();
      return &m_node;
    }
    bool operator==(const self_type& rhs) { return m_index == rhs.m_index; }
    bool operator!=(const self_type& rhs) { return m_index != rhs.m_index; }
  private:
    void fillNode() {
      m_node.begin = m_parent.m_begin + m_parent.m_points[m_index].m_begin;
      m_node.duration = m_parent.m_points[m_index].m_end;
      m_node.id = m_parent.m_points[m_index].m_id;
    }
    const PTimeline<config, N> &m_parent;
    unsigned m_index;
    Node m_node;
  };

  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, m_indexesUsed); }

private:
  friend class const_iterator;
  struct Point {
    Duration m_begin;
    Duration m_end;
    const char *m_id;
  };
  std::array<Point, N> m_points;
  TimePoint m_begin;
  TimePoint m_end;
  TimePoint m_last;
  const char *m_id;
  unsigned m_indexesUsed;
};

class PTimePoint
{
public:
  PTimePoint(const char *id)
    : m_index()
  {
    if (PTimelineBase::m_instance) {
      m_index = PTimelineBase::m_instance->startTimePoint(id);
    }
  }
  ~PTimePoint() {
    if (PTimelineBase::m_instance) {
      PTimelineBase::m_instance->endTimePoint(m_index);
    }
  }
private:
  PTimelineBase::Index m_index;
};

}

#endif //PROFILER_PROFILER_H
