#include "markdown_parser.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iostream>

std::vector<std::shared_ptr<InlineNode>> MarkdownParser::parseInline(std::string_view text) {
    std::vector<std::shared_ptr<InlineNode>> result;
    if (text.empty()) return result;

    size_t i = 0;
    while (i < text.size()) {
        // Find the first occurrence of any inline formatting delimiter
        size_t first_pos = std::string::npos;
        std::string delim = "";

        for (size_t j = i; j < text.size(); ++j) {
            if (j + 1 < text.size()) {
                std::string_view pair = text.substr(j, 2);
                if (pair == "**") { first_pos = j; delim = "**"; break; }
                if (pair == "__") { first_pos = j; delim = "__"; break; }
                if (pair == "~~") { first_pos = j; delim = "~~"; break; }
            }
            char c = text[j];
            if (c == '*' || c == '_' || c == '`' || c == '[') {
                first_pos = j;
                delim = std::string(1, c);
                break;
            }
        }

        if (first_pos == std::string::npos) {
            // No delimiters remaining, add rest as plain text
            result.push_back(std::make_shared<InlineNode>(InlineType::Text, std::string(text.substr(i))));
            break;
        }

        // Add any plain text before the delimiter
        if (first_pos > i) {
            result.push_back(std::make_shared<InlineNode>(InlineType::Text, std::string(text.substr(i, first_pos - i))));
        }

        // Try to match the closing delimiter
        size_t start_content = first_pos + delim.size();
        
        if (delim == "[") {
            // Link parsing: find matching ']'
            size_t bracket_count = 1;
            size_t r_bracket = std::string::npos;
            for (size_t k = start_content; k < text.size(); ++k) {
                if (text[k] == '[') bracket_count++;
                else if (text[k] == ']') {
                    bracket_count--;
                    if (bracket_count == 0) {
                        r_bracket = k;
                        break;
                    }
                }
            }

            if (r_bracket != std::string::npos && r_bracket + 1 < text.size() && text[r_bracket + 1] == '(') {
                size_t l_paren = r_bracket + 1;
                size_t r_paren = text.find(')', l_paren);
                if (r_paren != std::string::npos) {
                    // Match found!
                    std::string_view link_text = text.substr(start_content, r_bracket - start_content);
                    std::string_view link_url = text.substr(l_paren + 1, r_paren - (l_paren + 1));

                    auto link_node = std::make_shared<InlineNode>(InlineType::Link, std::string(link_url));
                    link_node->children = parseInline(link_text);
                    result.push_back(link_node);

                    i = r_paren + 1;
                    continue;
                }
            }
        } else {
            // Standard formatting delimiters
            size_t close_pos = text.find(delim, start_content);
            if (close_pos != std::string::npos) {
                std::string_view inner = text.substr(start_content, close_pos - start_content);
                std::shared_ptr<InlineNode> formatted_node;

                if (delim == "**") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Bold);
                    formatted_node->children = parseInline(inner);
                } else if (delim == "__") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Underline);
                    formatted_node->children = parseInline(inner);
                } else if (delim == "~~") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Strikethrough);
                    formatted_node->children = parseInline(inner);
                } else if (delim == "*") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Italic);
                    formatted_node->children = parseInline(inner);
                } else if (delim == "_") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Italic);
                    formatted_node->children = parseInline(inner);
                } else if (delim == "`") {
                    formatted_node = std::make_shared<InlineNode>(InlineType::Code, std::string(inner));
                }

                result.push_back(formatted_node);
                i = close_pos + delim.size();
                continue;
            }
        }

        // Delimiter was unmatched; treat it as plain text and advance
        result.push_back(std::make_shared<InlineNode>(InlineType::Text, std::string(text.substr(first_pos, delim.size()))));
        i = first_pos + delim.size();
    }
    return result;
}

std::vector<std::shared_ptr<BlockNode>> MarkdownParser::parse(const std::string& markdownText) {
    std::vector<std::shared_ptr<BlockNode>> blocks;
    std::vector<std::string> lines;
    
    // Split into lines and strip trailing carriage returns
    {
        std::string line;
        std::istringstream stream(markdownText);
        while (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            lines.push_back(line);
        }
    }

    size_t i = 0;
    std::shared_ptr<BlockNode> current_list = nullptr;

    while (i < lines.size()) {
        const auto& line = lines[i];

        // 1. Code Block Check
        if (line.rfind("```", 0) == 0) {
            current_list = nullptr; // Terminate list

            auto code_node = std::make_shared<BlockNode>(BlockType::CodeBlock);
            code_node->code_language = line.substr(3);

            // Trim leading spaces from language
            size_t lang_start = code_node->code_language.find_first_not_of(" \t");
            if (lang_start != std::string::npos) {
                code_node->code_language = code_node->code_language.substr(lang_start);
            } else {
                code_node->code_language.clear();
            }

            std::string code_content;
            i++;
            while (i < lines.size()) {
                if (lines[i].rfind("```", 0) == 0) {
                    break;
                }
                if (!code_content.empty()) {
                    code_content += "\n";
                }
                code_content += lines[i];
                i++;
            }
            code_node->raw_content = code_content;
            blocks.push_back(code_node);
            i++; // skip closing ```
            continue;
        }

        // Check for empty line
        size_t non_space = line.find_first_not_of(" \t");
        if (non_space == std::string::npos) {
            current_list = nullptr; // Empty line ends active list
            i++;
            continue;
        }

        std::string trimmed_line = line.substr(non_space);

        // 2. Heading Check
        if (trimmed_line[0] == '#') {
            current_list = nullptr;
            size_t hash_count = 0;
            while (hash_count < trimmed_line.size() && trimmed_line[hash_count] == '#') {
                hash_count++;
            }
            if (hash_count > 0 && hash_count <= 6 && hash_count < trimmed_line.size() && trimmed_line[hash_count] == ' ') {
                auto heading_node = std::make_shared<BlockNode>(BlockType::Heading);
                heading_node->heading_level = static_cast<int>(hash_count);
                heading_node->inline_children = parseInline(trimmed_line.substr(hash_count + 1));
                blocks.push_back(heading_node);
                i++;
                continue;
            }
        }

        // 3. Blockquote Check
        if (trimmed_line.rfind("> ", 0) == 0 || trimmed_line == ">") {
            current_list = nullptr;
            auto quote_node = std::make_shared<BlockNode>(BlockType::Blockquote);

            std::string quote_content;
            while (i < lines.size()) {
                size_t l_ns = lines[i].find_first_not_of(" \t");
                std::string l_trim = (l_ns == std::string::npos) ? "" : lines[i].substr(l_ns);

                if (l_trim.rfind("> ", 0) == 0) {
                    if (!quote_content.empty()) quote_content += "\n";
                    quote_content += l_trim.substr(2);
                } else if (l_trim == ">") {
                    if (!quote_content.empty()) quote_content += "\n";
                } else {
                    break;
                }
                i++;
            }

            quote_node->block_children = parse(quote_content);
            blocks.push_back(quote_node);
            continue;
        }

        // 4. Divider Check
        if (trimmed_line == "---" || trimmed_line == "***" || trimmed_line == "___") {
            current_list = nullptr;
            blocks.push_back(std::make_shared<BlockNode>(BlockType::Divider));
            i++;
            continue;
        }

        // 5. Unordered List Check
        bool is_ul = (trimmed_line.rfind("- ", 0) == 0 || trimmed_line.rfind("* ", 0) == 0);
        // 6. Ordered List Check
        bool is_ol = false;
        size_t dot_pos = std::string::npos;
        if (!is_ul && std::isdigit(trimmed_line[0])) {
            size_t digit_len = 0;
            while (digit_len < trimmed_line.size() && std::isdigit(trimmed_line[digit_len])) {
                digit_len++;
            }
            if (digit_len > 0 && digit_len < trimmed_line.size() && trimmed_line[digit_len] == '.') {
                if (digit_len + 1 < trimmed_line.size() && trimmed_line[digit_len + 1] == ' ') {
                    is_ol = true;
                    dot_pos = digit_len;
                }
            }
        }

        if (is_ul || is_ol) {
            std::string item_text = is_ul ? trimmed_line.substr(2) : trimmed_line.substr(dot_pos + 2);

            ListItem item;
            item.content = parseInline(item_text);

            if (current_list && current_list->is_ordered_list == is_ol) {
                current_list->list_items.push_back(item);
            } else {
                current_list = std::make_shared<BlockNode>(BlockType::List);
                current_list->is_ordered_list = is_ol;
                current_list->list_items.push_back(item);
                blocks.push_back(current_list);
            }
            i++;
            continue;
        }

        // 7. Paragraph Check (handles multi-line paragraphs)
        current_list = nullptr;
        auto para_node = std::make_shared<BlockNode>(BlockType::Paragraph);
        std::string para_text = trimmed_line;
        i++;
        while (i < lines.size()) {
            size_t l_ns = lines[i].find_first_not_of(" \t");
            if (l_ns == std::string::npos) {
                break; // Empty line ends paragraph
            }
            std::string l_trim = lines[i].substr(l_ns);

            // Check if next line starts a new block
            if (l_trim.rfind("```", 0) == 0 ||
                l_trim[0] == '#' ||
                l_trim.rfind("> ", 0) == 0 || l_trim == ">" ||
                l_trim == "---" || l_trim == "***" || l_trim == "___" ||
                l_trim.rfind("- ", 0) == 0 || l_trim.rfind("* ", 0) == 0) {
                break;
            }

            // Check if next line is ordered list item
            if (std::isdigit(l_trim[0])) {
                size_t dl = 0;
                while (dl < l_trim.size() && std::isdigit(l_trim[dl])) dl++;
                if (dl > 0 && dl < l_trim.size() && l_trim[dl] == '.' && dl + 1 < l_trim.size() && l_trim[dl + 1] == ' ') {
                    break;
                }
            }

            para_text += " " + l_trim;
            i++;
        }

        para_node->inline_children = parseInline(para_text);
        blocks.push_back(para_node);
    }

    return blocks;
}
