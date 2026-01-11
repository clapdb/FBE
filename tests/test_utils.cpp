/*
 * Copyright (C) STDB Holdings Limited
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * This file is part of FBE (Fast Binary Encoding).
 *
 * FBE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with FBE. If not, see <https://www.gnu.org/licenses/>.
 */
//
// Test utility functions
//

#include "test.h"

#include <string>
#include <vector>
#include <algorithm>

// Include utility functions from fbe.h (inline functions only)
namespace {

// Copy of is_escaped_name for testing
inline auto is_escaped_name(const std::string& name) -> bool {
    if (name.size() > 4 and name.starts_with("__") and name.ends_with("__"))
        return true;
    return false;
}

// Copy of extract_escaped_name_inplace for testing
inline void extract_escaped_name_inplace(std::string& name) {
    // is_escaped_name requires size > 4, so substr(2, size-4) is always valid
    if (!is_escaped_name(name))
        return;  // Invalid input - do nothing
    name.assign(name.substr(2, name.size() - 4));
}

// Copy of replace_all for testing
inline auto replace_all(std::string& inout, std::string_view what, std::string_view with) -> size_t
{
    if (what.empty())
        return 0;  // Avoid infinite loop with empty search string
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

// Copy of trim for testing
inline auto trim(const std::string &s) -> std::string
{
    auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
    auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

} // anonymous namespace

TEST_CASE("Utility: is_escaped_name", "[FBE][Utils]")
{
    // Valid escaped names
    REQUIRE(is_escaped_name("__name__") == true);
    REQUIRE(is_escaped_name("__x__") == true);
    REQUIRE(is_escaped_name("__test__") == true);
    REQUIRE(is_escaped_name("__very_long_name__") == true);

    // Invalid escaped names - too short
    REQUIRE(is_escaped_name("") == false);
    REQUIRE(is_escaped_name("_") == false);
    REQUIRE(is_escaped_name("__") == false);
    REQUIRE(is_escaped_name("___") == false);
    REQUIRE(is_escaped_name("____") == false);  // Exactly 4 chars, needs > 4

    // Invalid escaped names - missing prefix or suffix
    REQUIRE(is_escaped_name("name__") == false);
    REQUIRE(is_escaped_name("__name") == false);
    REQUIRE(is_escaped_name("_name_") == false);
    REQUIRE(is_escaped_name("name") == false);
}

TEST_CASE("Utility: extract_escaped_name_inplace", "[FBE][Utils]")
{
    // Valid escaped name extraction
    {
        std::string name = "__test__";
        extract_escaped_name_inplace(name);
        REQUIRE(name == "test");
    }

    {
        std::string name = "__x__";
        extract_escaped_name_inplace(name);
        REQUIRE(name == "x");
    }

    {
        std::string name = "__hello_world__";
        extract_escaped_name_inplace(name);
        REQUIRE(name == "hello_world");
    }

    // Invalid input - should not modify the string
    {
        std::string name = "not_escaped";
        extract_escaped_name_inplace(name);
        REQUIRE(name == "not_escaped");  // Unchanged
    }

    {
        std::string name = "";
        extract_escaped_name_inplace(name);
        REQUIRE(name == "");  // Unchanged
    }

    {
        std::string name = "____";  // Exactly 4 chars, not > 4
        extract_escaped_name_inplace(name);
        REQUIRE(name == "____");  // Unchanged
    }
}

TEST_CASE("Utility: replace_all", "[FBE][Utils]")
{
    // Basic replacement
    {
        std::string s = "hello world";
        size_t count = replace_all(s, "o", "0");
        REQUIRE(s == "hell0 w0rld");
        REQUIRE(count == 2);
    }

    // No matches
    {
        std::string s = "hello world";
        size_t count = replace_all(s, "x", "y");
        REQUIRE(s == "hello world");
        REQUIRE(count == 0);
    }

    // Empty search string
    {
        std::string s = "hello";
        [[maybe_unused]] size_t count = replace_all(s, "", "x");
        REQUIRE(s == "hello");  // Should not loop infinitely
    }

    // Replace with longer string
    {
        std::string s = "a-b-c";
        size_t count = replace_all(s, "-", "---");
        REQUIRE(s == "a---b---c");
        REQUIRE(count == 2);
    }

    // Replace with shorter string
    {
        std::string s = "aaa bbb ccc";
        size_t count = replace_all(s, "aaa", "a");
        REQUIRE(s == "a bbb ccc");
        REQUIRE(count == 1);
    }

    // Replace with empty string (deletion)
    {
        std::string s = "hello world";
        size_t count = replace_all(s, " ", "");
        REQUIRE(s == "helloworld");
        REQUIRE(count == 1);
    }

    // Multiple consecutive matches
    {
        std::string s = "aaaa";
        size_t count = replace_all(s, "aa", "b");
        REQUIRE(s == "bb");
        REQUIRE(count == 2);
    }
}

TEST_CASE("Utility: trim", "[FBE][Utils]")
{
    // Basic trim
    REQUIRE(trim("  hello  ") == "hello");
    REQUIRE(trim("\thello\t") == "hello");
    REQUIRE(trim("\nhello\n") == "hello");

    // No trim needed
    REQUIRE(trim("hello") == "hello");

    // All whitespace
    REQUIRE(trim("   ") == "");
    REQUIRE(trim("\t\n") == "");

    // Empty string
    REQUIRE(trim("") == "");

    // Leading only
    REQUIRE(trim("  hello") == "hello");

    // Trailing only
    REQUIRE(trim("hello  ") == "hello");

    // Whitespace in middle (should be preserved)
    REQUIRE(trim("  hello world  ") == "hello world");
}
