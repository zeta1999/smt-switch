#pragma once

#include "smt_defs.h"
#include "sort.h"

namespace smt {

/** \class LoggingSort
 *  An abstract class for logging created Sorts
 */
class LoggingSort : public AbsSort
{
 public:
  LoggingSort(SortKind sk, Sort s) : sk(sk), sort(s) {}
  virtual ~LoggingSort(){};
  // implementations
  Sort get_sort_kind() const override;
  bool compare(const Sort s) const override;
  // dispatch to underlying sort
  std::size_t hash() const override;

  // throw not implemented exception
  // for specific getters (will be
  // implemented by a subclass)
  uint64_t get_width() const override
  {
    throw NotImplementedException(
        "get_width not implemented by generic LoggingSort");
  }

  Sort get_indexsort() const override
  {
    throw NotImplementedException(
        "get_indexsort not implemented by generic LoggingSort");
  }

  Sort get_elemsort() const override
  {
    throw NotImplementedException(
        "get_elemsort not implemented by generic LoggingSort");
  }

  vector<Sort> get_domain_sorts() const override
  {
    throw NotImplementedException(
        "get_domain_sorts not implemented by generic LoggingSort");
  }

  Sort get_codomain_sort() const override
  {
    throw NotImplementedException(
        "get_codomain_sort not implemented by generic LoggingSort");
  }

 protected:
  SortKind sk;
  Sort sort;
};

class BVLoggingSort : public LoggingSort
{
 public:
  BVLoggingSort(Sort s, uint64_t width);
  ~BVLoggingSort();

  typedef LoggingSort super;

  uint64_t get_width() const override;

 protected:
  uint64_t width;
}

class ArrayLoggingSort : public LoggingSort
{
 public:
  ArrayLoggingSort(Sort s, Sort idxsort, Sort esort);
  ~ArrayLoggingSort();

  typedef LoggingSort super;

  Sort get_indexsort() const override;
  Sort get_elemsort() const override;

 protected:
  Sort indexsort;
  Sort elemsort;
};

class FunctionLoggingSort : public LoggingSort
{
 public:
  FunctionLoggingSort(Sort s, std::vector<Sort> sorts, Sort rsort);
  ~FunctionLoggingSort();

  typedef LoggingSort super;

  std::vector<Sort> get_domain_sorts() const ovverride;
  Sort get_codomain_sort() const override;

 protected:
  std::vector<Sort> domain_sorts;
  Sort codomain_sort;
};

}  // namespace smt
