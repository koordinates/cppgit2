#pragma once
#include <cppgit2/git_exception.hpp>
#include <cppgit2/bitmask_operators.hpp>
#include <cppgit2/libgit2_api.hpp>
#include <cppgit2/oid.hpp>
#include <cppgit2/ownership.hpp>
#include <git2.h>
#include <string>

namespace cppgit2 {

class submodule : public libgit2_api {
public:
  // Default construction
  submodule();

  // Construct from libgit2 C ptr
  // If owned by user, will be free'd in the destructor
  submodule(git_submodule *c_ptr, ownership owner = ownership::libgit2);

  // Cleanup submodule ptr if owned by user
  ~submodule();

  // Just like "git submodule init", this copies information about the submodule
  // into ".git/config"
  void init(bool overwrite);

  // Resolve the setup of a new git submodule.
  //
  // This should be called on a submodule once you have called add setup and
  // done the clone of the submodule. This adds the .gitmodules file and the
  // newly cloned submodule to the index to be ready to be committed (but
  // doesn't actually do the commit).
  void resolve_setup();

  // Add current submodule HEAD commit to index of superproject.
  void add_to_index(bool write_index);

  // Get the branch for the submodule.
  std::string branch_name() const;

  enum class recurse {
    no = 0,  // do not recurse into submodules
    yes = 1, // recurse into submodules
    on_demand =
        2 // recurse into submodules when commit not already in local clone
  };

  // Read the fetchRecurseSubmodules rule for a submodule.
  recurse recurse_submodules_option() const;

  // Get the OID for the submodule in the current HEAD tree.
  oid head_id() const;

  enum class ignore {
    unspecified = -1, // use the submodule's configuration
    none = 1,         // any change or untracked == dirty
    untracked = 2,    // dirty if tracked files change
    dirty = 3,        // only dirty if HEAD moved
    all = 4           // never dirty
  };

  // Get the ignore rule that will be used for the submodule.
  ignore ignore_option() const;

  // Set up the subrepository for a submodule in preparation for clone.
  // This function can be called to init and set up a submodule repository 
  // from a submodule in preparation to clone it from its remote.
  //
  // @param use_gitlink: Should the workdir contain a gitlink to the repo 
  //                     in .git/modules vs. repo directly in workdir.
  class repository initialize_repository(bool use_gitlink);

  // Submodule update values
  // These values represent settings for the `submodule.$name.update`
  // configuration value which says how to handle `git submodule update` for
  // this submodule.  The value is usually set in the ".gitmodules" file and
  // copied to ".git/config" when the submodule is initialized.
  enum class update {
    checkout = 1,
    rebase = 2,
    merge = 3,
    none = 4,
    default_ = 0
  };

  update update_strategy() const;

  // Get the OID for the submodule in the index.
  oid index_id() const;

  // Get the name of submodule.
  std::string name() const;

  // Get the path to the submodule.
  //
  // The path is almost always the same as the submodule name,
  // but the two are actually not required to match.
  std::string path() const;

  // Reread submodule info from config, index, and HEAD.
  void reload(bool force);

  // Open the repository for a submodule.
  class repository open_repository();

  // Get the containing repository for a submodule.
  class repository owner();

  // Copy submodule remote info into submodule repo.
  void sync();

  // Get the URL for the submodule.
  std::string url() const;

  enum class status {
    in_head           = (1u << 0),
    in_index          = (1u << 1),
    in_config         = (1u << 2),
    in_wd             = (1u << 3),
    index_added       = (1u << 4),
    index_deleted     = (1u << 5),
    index_modified    = (1u << 6),
    wd_uninitialized  = (1u << 7),
    wd_added          = (1u << 8),
    wd_deleted        = (1u << 9),
    wd_modified       = (1u << 10),
    wd_index_modified = (1u << 11),
    wd_wd_modified    = (1u << 12),
    wd_untracked      = (1u << 13)
  };

  // Get the locations of submodule information.
  status location_status() const;

  // Access libgit2 C ptr
  const git_submodule *c_ptr() const;

private:
  friend class repository;
  git_submodule *c_ptr_;
  ownership owner_;
};
ENABLE_BITMASK_OPERATORS(submodule::status);

} // namespace cppgit2