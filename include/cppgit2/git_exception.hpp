#pragma once
#include <exception>
#include <git2.h>
#include <string>

#include <variant>

namespace cppgit2 {

template <typename tResult>
class git_result
{
public:
    git_result(tResult&& aResult)
    {
        mData.emplace<tResult>(std::move(aResult));
    }
    
    git_result()
    {
        mData = git_error_last();
    }

    bool HasResult()
    {
        if (mData.index() == 1) //error
            return false;
        else
            return true;
    }

    tResult& Result()
    {
        return std::get<tResult>(mData);
    }

    tResult&& Move()
    {
        return std::get<tResult>(std::move(mData));
    }

private:
    std::variant<tResult, const git_error*> mData;
};

class git_exception : public std::exception {
public:
  git_exception() {
    auto error = git_error_last();
    message_ = error ? error->message : "unknown error";
  }
  git_exception(const char *message) : message_(message) {}
  git_exception(const std::string &message) : message_(message.c_str()) {}
  virtual ~git_exception() throw() {}
  virtual const char *what() const throw() { return message_; }

  static void clear() { git_error_clear(); }

protected:
  const char *message_;
};

} // namespace cppgit2
