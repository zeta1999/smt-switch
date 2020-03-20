#include "logging_sort.h"

using namespace std;

namespace smt {

/* Helper functions */
Sort make_logging_sort(SortKind sk, Sort s)
{
  if (sk != BOOL && sk != INT && sk != REAL)
  {
    throw IncorrectUsageException("Can't create sort from " + to_string(sk));
  }
  Sort loggingsort(new LoggingSort(sk, s));
  return loggingsort;
}

Sort make_logging_sort(SortKind sk, Sort s, uint64_t width)
{
  if (sk != BV)
  {
    throw IncorrectUsageException("Can't create sort from " + to_string(sk)
                                  + " and " + ::std::to_string(width));
  }
  Sort loggingsort(new BVLoggingSort(s, width));
  return loggingsort;
}

Sort make_logging_sort(SortKind sk, Sort s, Sort sort1)
{
  throw IncorrectUsageException(
      "No currently supported sort is created with a single sort argument");
}

Sort make_logging_sort(SortKind sk, Sort s, Sort sort1, Sort sort2)
{
  Sort loggingsort;
  if (sk == ARRAY)
  {
    loggingsort = Sort(new ArrayLoggingSort(s, sort1, sort2));
  }
  else if (sk == FUNCTION)
  {
    loggingsort = Sort(new FunctionLoggingSort(s, SortVec{ sort1 }, sort2));
  }
  else
  {
    throw IncorrectUsageException("Can't make sort from " + to_string(sk) + " "
                                  + sort1->to_string() + " "
                                  + sort2->to_string());
  }
  return loggingsort;
}

Sort make_logging_sort(SortKind sk, Sort s, Sort sort1, Sort sort2, Sort sort3)
{
  if (sk == FUNCTION)
  {
    Sort loggingsort(
        new FunctionLoggingSort(s, SortVec{ sort1, sort2 }, sort3));
    return loggingsort;
  }
  else
  {
    throw IncorrectUsageException(
        "Can't make sort from " + to_string(sk) + " " + sort1->to_string() + " "
        + sort2->to_string() + " " + sort3->to_string());
  }
}

Sort make_logging_sort(SortKind sk, Sort s, SortVec sorts)
{
  if (sk == FUNCTION)
  {
    Sort return_sort = sorts.back();
    sorts.pop_back();
    Sort loggingsort(new FunctionLoggingSort(s, sorts, return_sort));
    return loggingsort;
  }
  else
  {
    std::string msg("Can't make sort from ");
    msg += to_string(sk);
    for (auto ss : sorts)
    {
      msg += " " + ss->to_string();
    }
    throw IncorrectUsageException(msg);
  }
}

// implementations
SortKind LoggingSort::get_sort_kind() const { return sk; }

bool LoggingSort::compare(const Sort s) const
{
  SortKind other_sk = s->get_sort_kind();
  if (sk != other_sk)
  {
    return false;
  }

  switch (sk)
  {
    case BOOL:
    case INT:
    case REAL: { return true;
    }
    case BV: { return get_width() == s->get_width();
    }
    case ARRAY:
    {
      return (get_indexsort() == s->get_indexsort())
             && (get_elemsort() == s->get_elemsort());
    }
    case FUNCTION:
    {
      SortVec domain_sorts = get_domain_sorts();
      SortVec other_domain_sorts = s->get_domain_sorts();
      Sort return_sort = get_codomain_sort();
      Sort other_return_sort = s->get_codomain_sort();

      if (domain_sorts.size() != other_domain_sorts.size()
          || return_sort != other_return_sort)
      {
        return false;
      }

      for (size_t i = 0; i < domain_sorts.size(); i++)
      {
        if (domain_sorts[i] != other_domain_sorts[i])
        {
          return false;
        }
      }

      return true;
    }
    case NUM_SORT_CONS:
    {
      // null sorts should not be equal
      return false;
    }
  }
}

size_t LoggingSort::hash() const { return sort->hash(); }

// BVLoggingSort

BVLoggingSort::BVLoggingSort(Sort s, uint64_t width)
    : super(BV, s), width(width)
{
}

BVLoggingSort::~BVLoggingSort() {}

uint64_t BVLoggingSort::get_width() const { return width; }

// ArrayLoggingSort

ArrayLoggingSort::ArrayLoggingSort(Sort s, Sort idxsort, Sort esort)
    : super(ARRAY, s), indexsort(idxsort), elemsort(esort)
{
}

ArrayLoggingSort::~ArrayLoggingSort() {}

Sort ArrayLoggingSort::get_indexsort() const { return indexsort; }

Sort ArrayLoggingSort::get_elemsort() const { return elemsort; }

// FunctionLoggingSort

FunctionLoggingSort::FunctionLoggingSort(Sort s, SortVec sorts, Sort rsort)
    : super(FUNCTION, s), domain_sorts(sorts), codomain_sort(rsort)
{
}

FunctionLoggingSort::~FunctionLoggingSort() {}

SortVec FunctionLoggingSort::get_domain_sorts() const
{
  return domain_sorts;
}

Sort FunctionLoggingSort::get_codomain_sort() const { return codomain_sort; }

// methods dispatched to underlying sort

}  // namespace smt
