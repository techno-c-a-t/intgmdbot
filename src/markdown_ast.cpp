#include "markdown_ast.h"
#include <algorithm>
#include <sstream>

namespace {

std::string escapeText(const std::string &text) {
  const std::string reserved = "_*[]()~`>#+-=|{}.!";
  std::string escaped;
  escaped.reserve(text.size() * 2);
  for (char c : text) {
    if (c == '\\') {
      escaped += "\\\\";
    } else if (reserved.find(c) != std::string::npos) {
      escaped += '\\';
      escaped += c;
    } else {
      escaped += c;
    }
  }
  return escaped;
}

std::string escapeCode(const std::string &text) {
  std::string escaped;
  escaped.reserve(text.size() * 2);
  for (char c : text) {
    if (c == '\\') {
      escaped += "\\\\";
    } else if (c == '`') {
      escaped += "\\`";
    } else {
      escaped += c;
    }
  }
  return escaped;
}

std::string escapeLink(const std::string &text) {
  std::string escaped;
  escaped.reserve(text.size() * 2);
  for (char c : text) {
    if (c == '\\') {
      escaped += "\\\\";
    } else if (c == ')') {
      escaped += "\\)";
    } else {
      escaped += c;
    }
  }
  return escaped;
}

} // namespace

std::string InlineNode::toMarkdownV2() const {
  switch (type) {
  case InlineType::Text:
    return escapeText(text);
  case InlineType::Bold: {
    std::string res = "*";
    for (const auto &child : children) {
      res += child->toMarkdownV2();
    }
    res += "*";
    return res;
  }
  case InlineType::Italic: {
    std::string res = "_";
    for (const auto &child : children) {
      res += child->toMarkdownV2();
    }
    res += "_";
    return res;
  }
  case InlineType::Underline: {
    std::string res = "__";
    for (const auto &child : children) {
      res += child->toMarkdownV2();
    }
    res += "__";
    return res;
  }
  case InlineType::Strikethrough: {
    std::string res = "~";
    for (const auto &child : children) {
      res += child->toMarkdownV2();
    }
    res += "~";
    return res;
  }
  case InlineType::Code:
    return "`" + escapeCode(text) + "`";
  case InlineType::Link: {
    std::string res = "[";
    for (const auto &child : children) {
      res += child->toMarkdownV2();
    }
    res += "](" + escapeLink(text) + ")";
    return res;
  }
  }
  return "";
}

std::string BlockNode::toMarkdownV2() const {
  std::ostringstream ss;
  switch (type) {
  case BlockType::Paragraph: {
    for (const auto &child : inline_children) {
      ss << child->toMarkdownV2();
    }
    ss << "\n";
    break;
  }
  case BlockType::Heading: {
    // Underline and Bold for Level 1, Bold for Level 2, Italic and Bold for
    // Level 3+
    if (heading_level == 1) {
      ss << "__*";
      for (const auto &child : inline_children)
        ss << child->toMarkdownV2();
      ss << "*__\n";
    } else if (heading_level == 2) {
      ss << "*";
      for (const auto &child : inline_children)
        ss << child->toMarkdownV2();
      ss << "*\n";
    } else {
      ss << "_*";
      for (const auto &child : inline_children)
        ss << child->toMarkdownV2();
      ss << "*_\n";
    }
    break;
  }
  case BlockType::List: {
    int count = 1;
    for (const auto &item : list_items) {
      if (is_ordered_list) {
        ss << count << "\\. ";
        count++;
      } else {
        ss << "• ";
      }
      for (const auto &child : item.content) {
        ss << child->toMarkdownV2();
      }
      ss << "\n";
    }
    break;
  }
  case BlockType::Blockquote: {
    // Gather all content and prefix each line with '>'
    std::string inner_content;
    if (!inline_children.empty()) {
      for (const auto &child : inline_children) {
        inner_content += child->toMarkdownV2();
      }
    } else {
      for (const auto &child : block_children) {
        inner_content += child->toMarkdownV2();
      }
    }

    std::istringstream inner_ss(inner_content);
    std::string line;
    while (std::getline(inner_ss, line)) {
      ss << "> " << line << "\n";
    }
    break;
  }
  case BlockType::CodeBlock: {
    ss << "```" << code_language << "\n"
       << escapeCode(raw_content) << "\n```\n";
    break;
  }
  case BlockType::Divider: {
    ss << "\\-\\-\\-\\-\\-\\-\\-\\-\\-\\-\n";
    break;
  }
  }
  return ss.str();
}
