#pragma once
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

enum class InlineType {
    Text,
    Bold,
    Italic,
    Underline,
    Strikethrough,
    Code,
    Link
};

struct InlineNode {
    InlineType type;
    std::string text; // Used for Text, Code, and Link URL
    std::vector<std::shared_ptr<InlineNode>> children; // Used for Bold, Italic, Underline, Strikethrough, Link text

    InlineNode(InlineType t) : type(t) {}
    InlineNode(InlineType t, std::string txt) : type(t), text(std::move(txt)) {}

    std::string toMarkdownV2() const;
};

enum class BlockType {
    Paragraph,
    Heading,
    List,
    Blockquote,
    CodeBlock,
    Divider
};

struct ListItem {
    std::vector<std::shared_ptr<InlineNode>> content;
};

struct BlockNode {
    BlockType type;
    int heading_level = 0; // 1 to 6
    bool is_ordered_list = false; // true = ordered (1.), false = unordered (-/*)
    std::string code_language; // For CodeBlock
    std::string raw_content; // For CodeBlock

    std::vector<std::shared_ptr<InlineNode>> inline_children; // For Paragraph, Heading, Blockquote
    std::vector<ListItem> list_items; // For List
    std::vector<std::shared_ptr<BlockNode>> block_children; // For Blockquote

    BlockNode(BlockType t) : type(t) {}

    std::string toMarkdownV2() const;
};
