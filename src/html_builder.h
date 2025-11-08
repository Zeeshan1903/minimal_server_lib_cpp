
#include <string>

class HtmlBuild {
private:
    std::string result;

public:
    HtmlBuild& start() {
        result += "<!DOCTYPE html>\n<html>\n<head>\n";
        return *this;
    }

    HtmlBuild& title(std::string title) {
        result += "<title>" + title + "</title>\n";
        return *this;
    }

    HtmlBuild& para(std::string p) {
        result += "<p>" + p + "</p>\n";
        return *this;
    }

    HtmlBuild& startBody() {
        result += "</head>\n<body>\n";
        return *this;
    }

    HtmlBuild& heading(std::string head) {
        result += "<h>" + head + "</h>";
        return *this;
    }

    HtmlBuild& link(std::string url, std::string text) {
        result += "<a href=\"" + url + "\">" + text + "</a>\n";
        return *this;
    }

    HtmlBuild& lineBreak() {
        result += "<br>\n";
        return *this;
    }

    HtmlBuild& startForm(std::string action, std::string method = "POST") {
        result += "<form action=\"" + action + "\" method=\"" + method + "\">\n";
        return *this;
    }

    HtmlBuild& input(std::string type, std::string name, std::string placeholder = "") {
        result += "<input type=\"" + type + "\" name=\"" + name + "\"";
        if (!placeholder.empty()) {
            result += " placeholder=\"" + placeholder + "\"";
        }
        result += ">\n";
        return *this;
    }

    HtmlBuild& button(std::string text, std::string type = "button") {
        result += "<button type=\"" + type + "\">" + text + "</button>\n";
        return *this;
    }

    HtmlBuild& endForm() {
        result += "</form>\n";
        return *this;
    }

    HtmlBuild& endDocument() {
        result += "</body>\n</html>";
        return *this;
    }

    std::string build() {
        return result;
    }
};
