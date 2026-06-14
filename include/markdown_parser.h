#pragma once
#include <string>
#include <vector>
#include <memory>
#include "markdown_ast.h"

class MarkdownParser {
public:
    // Main parse entry point.
    static std::vector<std::shared_ptr<BlockNode>> parse(const std::string& markdownText);

    // Parses inline formatting elements (bold, italic, links, etc.) inside text.
    static std::vector<std::shared_ptr<InlineNode>> parseInline(std::string_view text);
};
