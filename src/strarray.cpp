#include <cppgit2/strarray.hpp>
#include <cstdlib>
#include <cstring>

namespace cppgit2 {

strarray::strarray() {
  c_struct_.count = 0;
  c_struct_.strings = nullptr;
}

strarray::strarray(const std::vector<std::string> &strings) {
  std::vector<const char*> stringPointers;
  for (auto& ptr : strings)
    stringPointers.push_back(ptr.c_str());

  git_strarray temp;
  temp.strings = const_cast<char**>(stringPointers.data());
  temp.count = stringPointers.size();

  git_strarray_copy(&c_struct_, &temp);
}

strarray::strarray(const git_strarray *c_ptr) {
  git_strarray_copy(&c_struct_, c_ptr);
}

strarray::~strarray() {
  if (c_struct_.count)
    git_strarray_free(&c_struct_);
}

strarray::strarray(strarray&& other) {
  c_struct_.count = other.c_struct_.count;
  c_struct_.strings = other.c_struct_.strings;
  other.c_struct_.count = 0;
  other.c_struct_.strings = nullptr;
}

strarray& strarray::operator= (strarray&& other) {
  if (other.c_struct_.strings != c_struct_.strings) {
    c_struct_.count = other.c_struct_.count;
    c_struct_.strings = other.c_struct_.strings;
    other.c_struct_.count = 0;
    other.c_struct_.strings = nullptr;
  }
  return *this;
}

strarray strarray::copy() const {
  return *this;
}

strarray::strarray(strarray const& other) {
  if (git_strarray_copy(&c_struct_, &other.c_struct_))
    throw git_exception();
}

std::vector<std::string> strarray::to_vector() const {
  std::vector<std::string> result{};
  for (auto tag : *this)
    result.push_back(tag);
  return result;
}

const git_strarray *strarray::c_ptr() const { return &c_struct_; }

} // namespace cppgit2
