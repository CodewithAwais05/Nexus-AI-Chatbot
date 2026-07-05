#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <functional>
#include <map>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #include <winhttp.h>  // Requires Windows SDK - commented out for demo
#define PLATFORM_WINDOWS 1
#else
#include <unistd.h>
#define PLATFORM_WINDOWS 0
#endif

// ============================================================================
//  COLORS
// ============================================================================
namespace C
{
    const std::string R = "\033[0m";
    const std::string B = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string RED = "\033[31m";
    const std::string GRN = "\033[32m";
    const std::string YLW = "\033[33m";
    const std::string BLU = "\033[34m";
    const std::string MAG = "\033[35m";
    const std::string CYN = "\033[36m";
    const std::string WHT = "\033[37m";
    const std::string BRED = "\033[91m";
    const std::string BGRN = "\033[92m";
    const std::string BYLW = "\033[93m";
    const std::string BBLU = "\033[94m";
    const std::string BMAG = "\033[95m";
    const std::string BCYN = "\033[96m";
    const std::string BWHT = "\033[97m";
    const std::string BGRD = "\033[41m";
    const std::string BGBL = "\033[44m";
    const std::string BGGR = "\033[42m";
    const std::string BGMG = "\033[45m";
}

// ============================================================================
//  UI HELPERS
// ============================================================================
namespace UI
{
    void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#endif
    }

    void hline(char ch = '-', int w = 76, const std::string &col = C::CYN)
    {
        std::cout << col;
        for (int i = 0; i < w; ++i)
            std::cout << ch;
        std::cout << C::R << "\n";
    }

    void dline(int w = 76) { hline('=', w, C::BCYN); }

    void title(const std::string &t, const std::string &col = C::BCYN)
    {
        int w = 76;
        dline(w);
        int pad = (w - 2 - (int)t.size()) / 2;
        int rpad = w - 2 - pad - (int)t.size();
        std::cout << col << C::B << "|";
        for (int i = 0; i < pad; ++i)
            std::cout << " ";
        std::cout << t;
        for (int i = 0; i < rpad; ++i)
            std::cout << " ";
        std::cout << "|" << C::R << "\n";
        dline(w);
    }

    void ok(const std::string &m) { std::cout << C::BGRN << C::B << "  [OK]  " << C::R << m << "\n"; }
    void err(const std::string &m) { std::cout << C::BRED << C::B << " [ERR]  " << C::R << m << "\n"; }
    void wrn(const std::string &m) { std::cout << C::BYLW << C::B << "  [!]   " << C::R << m << "\n"; }
    void inf(const std::string &m) { std::cout << C::BBLU << "  [i]   " << C::R << m << "\n"; }

    std::string input(const std::string &prompt, const std::string &col = C::BCYN)
    {
        std::cout << col << C::B << "  >> " << prompt << C::R << C::CYN << " : " << C::R;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    std::string password(const std::string &prompt)
    {
        std::cout << C::BMAG << C::B << "  >> " << prompt << C::R << C::MAG << " : " << C::R;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void pause()
    {
        std::cout << "\n"
                  << C::DIM << "  Press ENTER to continue..." << C::R;
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    int menu(int lo, int hi)
    {
        std::string s;
        while (true)
        {
            std::cout << C::BYLW << C::B << "\n  >> Choice [" << lo << "-" << hi << "]"
                      << C::R << C::YLW << " : " << C::R;
            std::getline(std::cin, s);
            try
            {
                int v = std::stoi(s);
                if (v >= lo && v <= hi)
                    return v;
            }
            catch (...)
            {
            }
            err("Enter a number between " + std::to_string(lo) + " and " + std::to_string(hi));
        }
    }

    void section(const std::string &s, const std::string &col = C::BCYN)
    {
        std::cout << "\n"
                  << col << C::B << "  [ " << s << " ]" << C::R << "\n";
        hline('-', 76, col);
    }

    void menuBox(const std::string &heading,
                 const std::vector<std::pair<std::string, std::string>> &items,
                 const std::string &col = C::BCYN)
    {
        section(heading, col);
        for (auto &p : items)
        {
            std::cout << col << "   " << C::B << std::setw(5) << std::left
                      << ("[" + p.first + "]")
                      << C::R << C::BWHT << " " << p.second << C::R << "\n";
        }
    }

    void kv(const std::string &k, const std::string &v,
            const std::string &kc = C::YLW, const std::string &vc = C::BWHT)
    {
        std::cout << kc << "  " << std::setw(22) << std::left << k << vc << v << C::R << "\n";
    }

    void para(const std::string &text, int maxcol = 70)
    {
        std::istringstream ss(text);
        std::string word;
        int col = 0;
        std::cout << "  ";
        while (ss >> word)
        {
            if (col + (int)word.size() + 1 > maxcol)
            {
                std::cout << "\n  ";
                col = 0;
            }
            std::cout << word << " ";
            col += (int)word.size() + 1;
        }
        std::cout << "\n";
    }

    void spinner(const std::string &msg)
    {
        std::cout << C::DIM << "\n  [..] " << msg << C::R << "\n";
    }

    std::string badge(const std::string &text, bool good)
    {
        return (good ? C::BGGR + C::BWHT : C::BGRD + C::BWHT) + C::B + " " + text + " " + C::R;
    }

    // Typing-style output for AI responses
    void typeOut(const std::string &text, int maxcol = 68)
    {
        std::istringstream ss(text);
        std::string word;
        int col = 0;
        std::cout << C::BWHT << "  ";
        while (ss >> word)
        {
            if (col + (int)word.size() + 1 > maxcol)
            {
                std::cout << "\n  ";
                col = 0;
            }
            std::cout << word << " ";
            col += (int)word.size() + 1;
        }
        std::cout << C::R << "\n";
    }
}

// ============================================================================
//  JSON EXTRACTOR
// ============================================================================
class Json
{
public:
    static std::string str(const std::string &j, const std::string &key)
    {
        std::string s = "\"" + key + "\"";
        size_t p = j.find(s);
        if (p == std::string::npos)
            return "";
        p = j.find(':', p);
        if (p == std::string::npos)
            return "";
        p = j.find_first_not_of(" \t\n\r", p + 1);
        if (p == std::string::npos)
            return "";
        if (j[p] == '"')
        {
            size_t a = p + 1, b = j.find('"', a);
            return (b == std::string::npos) ? "" : j.substr(a, b - a);
        }
        size_t e = j.find_first_of(",}\n", p);
        if (e == std::string::npos)
            e = j.size();
        std::string v = j.substr(p, e - p);
        v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());
        return v;
    }

    static std::string esc(const std::string &s)
    {
        std::string o;
        for (char c : s)
        {
            if (c == '"')
                o += "\\\"";
            else if (c == '\n')
                o += "\\n";
            else if (c == '\r')
                o += "\\r";
            else if (c == '\\')
                o += "\\\\";
            else
                o += c;
        }
        return o;
    }

    static std::string unesc(const std::string &s)
    {
        std::string o;
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (s[i] == '\\' && i + 1 < s.size())
            {
                ++i;
                if (s[i] == 'n')
                    o += '\n';
                else if (s[i] == '"')
                    o += '"';
                else if (s[i] == '\\')
                    o += '\\';
                else if (s[i] == 't')
                    o += '\t';
                else
                {
                    o += '\\';
                    o += s[i];
                }
            }
            else
            {
                o += s[i];
            }
        }
        return o;
    }

    // Robustly extract first text/content value from AI response
    static std::string firstText(const std::string &resp)
    {
        // Try multiple key patterns used by different AI providers
        for (const auto &key : {"\"text\"", "\"content\"", "\"message\""})
        {
            size_t p = resp.find(key);
            if (p == std::string::npos)
                continue;
            p = resp.find(':', p);
            if (p == std::string::npos)
                continue;
            p = resp.find('"', p);
            if (p == std::string::npos)
                continue;
            // Check if value is a string or an object
            if (p + 1 < resp.size() && resp[p + 1] == '{')
                continue;
            size_t start = p + 1, end = start;
            while (end < resp.size())
            {
                if (resp[end] == '\\')
                {
                    end += 2;
                    continue;
                }
                if (resp[end] == '"')
                    break;
                ++end;
            }
            std::string result = unesc(resp.substr(start, end - start));
            if (!result.empty() && result.size() > 3)
                return result;
        }
        return resp;
    }

    // Check if response contains an error field
    static bool hasError(const std::string &resp)
    {
        return resp.find("\"error\"") != std::string::npos ||
               resp.find("\"Error\"") != std::string::npos;
    }
};

// ============================================================================
//  HTTP CLIENT  (stub implementation - WinHTTP requires Windows SDK)
// ============================================================================
class Http
{
public:
    // Returns true if the HTTP backend is available on this build
    static bool backendAvailable()
    {
        return false; // Stub: HTTP backend not available without WinHTTP SDK
    }

    // POST with detailed error output (stub)
    static std::string post(const std::string &url,
                            const std::string &body,
                            const std::vector<std::string> &hdrs,
                            std::string &outErr)
    {
        outErr = "HTTP functionality requires Windows SDK (winhttp.h). "
                 "This build uses stub implementation.";
        return "";
    }

    // Convenience overload (discards error string)
    static std::string post(const std::string &url,
                            const std::string &body,
                            const std::vector<std::string> &hdrs)
    {
        std::string e;
        return post(url, body, hdrs, e);
    }
};

// ============================================================================
//  AI PROVIDER DETECTION & CHATBOT
//    gsk_...    -> Groq
//    AIza...    -> Google Gemini
//    sk-ant-... -> Anthropic
//    sk-...     -> OpenAI
//    other+URL  -> Custom OpenAI-compatible
// ============================================================================
enum class AiProv
{
    GROQ,
    GEMINI,
    ANTHROPIC,
    OPENAI,
    CUSTOM,
    NONE
};

class Chatbot
{
    std::string apiKey_, customUrl_;
    AiProv prov_;

    AiProv detect(const std::string &k)
    {
        if (k.empty())
            return AiProv::NONE;
        if (k.size() >= 4 && k.substr(0, 4) == "gsk_")
            return AiProv::GROQ;
        if (k.size() >= 4 && k.substr(0, 4) == "AIza")
            return AiProv::GEMINI;
        if (k.size() >= 7 && k.substr(0, 7) == "sk-ant")
            return AiProv::ANTHROPIC;
        if (k.size() >= 3 && k.substr(0, 3) == "sk-")
            return AiProv::OPENAI;
        return AiProv::CUSTOM;
    }

    // Build request bodies for each provider
    std::string oaiBody(const std::string &sys, const std::string &usr, const std::string &model)
    {
        return "{\"model\":\"" + model + "\","
                                         "\"max_tokens\":800,"
                                         "\"messages\":["
                                         "{\"role\":\"system\",\"content\":\"" +
               Json::esc(sys) + "\"},"
                                "{\"role\":\"user\",\"content\":\"" +
               Json::esc(usr) + "\"}"
                                "]}";
    }

    std::string antBody(const std::string &sys, const std::string &usr)
    {
        return "{\"model\":\"claude-haiku-4-5-20251001\","
               "\"max_tokens\":800,"
               "\"system\":\"" +
               Json::esc(sys) + "\","
                                "\"messages\":[{\"role\":\"user\",\"content\":\"" +
               Json::esc(usr) + "\"}]}";
    }

    std::string gemBody(const std::string &sys, const std::string &usr)
    {
        return "{\"system_instruction\":{\"parts\":[{\"text\":\"" + Json::esc(sys) + "\"}]},"
                                                                                     "\"contents\":[{\"parts\":[{\"text\":\"" +
               Json::esc(usr) + "\"}]}]}";
    }

    // Offline mock responses for demo mode
    std::string mock(const std::string &q)
    {
        std::string l = q;
        std::transform(l.begin(), l.end(), l.begin(), ::tolower);
        if (l.find("hello") != std::string::npos || l.find("hi") != std::string::npos)
            return "Hello! I am NexusChat, your AI assistant. How may I help you today?";
        if (l.find("help") != std::string::npos)
            return "I can answer questions, assist with writing, brainstorming, coding, and much more. Just ask!";
        if (l.find("time") != std::string::npos)
            return "I don't have access to real-time data in demo mode. Please set an AI API key.";
        if (l.find("code") != std::string::npos || l.find("program") != std::string::npos)
            return "I can help with code! In live mode I can write, debug, and explain programs. Set your API key to unlock full capability.";
        if (l.find("bye") != std::string::npos || l.find("exit") != std::string::npos)
            return "Goodbye! It was great chatting with you.";
        return "I'm running in DEMO mode. Set an AI API key in Settings to get real AI responses. "
               "Supported providers: Groq (free), Gemini (free), Anthropic, OpenAI.";
    }

public:
    Chatbot() : prov_(AiProv::NONE) {}

    void configure(const std::string &key, const std::string &customUrl = "")
    {
        apiKey_ = key;
        customUrl_ = customUrl;
        prov_ = detect(key);
        if (prov_ == AiProv::CUSTOM && customUrl.empty())
            prov_ = AiProv::NONE;
    }

    const std::string &key() const { return apiKey_; }
    const std::string &customUrl() const { return customUrl_; }
    bool isMock() const { return prov_ == AiProv::NONE; }

    std::string provName() const
    {
        switch (prov_)
        {
        case AiProv::GROQ:
            return "Groq (llama-3.3-70b)";
        case AiProv::GEMINI:
            return "Google Gemini 1.5 Flash";
        case AiProv::ANTHROPIC:
            return "Anthropic Claude Haiku";
        case AiProv::OPENAI:
            return "OpenAI GPT-3.5-Turbo";
        case AiProv::CUSTOM:
            return "Custom Endpoint";
        default:
            return "DEMO (no key)";
        }
    }

    // Send a message and return the AI reply; fills outErr on failure
    std::string ask(const std::string &userMsg,
                    const std::string &systemPrompt,
                    std::string &outErr)
    {
        outErr.clear();
        if (prov_ == AiProv::NONE)
            return mock(userMsg);

        // Verify HTTP backend before trying
        if (!Http::backendAvailable())
        {
            outErr = "HTTP backend unavailable on this build.";
            return "[ERR] " + outErr;
        }

        std::string body, url, resp;
        std::vector<std::string> hdrs;

        switch (prov_)
        {
        case AiProv::GROQ:
            body = oaiBody(systemPrompt, userMsg, "llama-3.3-70b-versatile");
            url = "https://api.groq.com/openai/v1/chat/completions";
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            break;

        case AiProv::GEMINI:
            body = gemBody(systemPrompt, userMsg);
            url = "https://generativelanguage.googleapis.com/v1beta/models/"
                  "gemini-1.5-flash:generateContent?key=" +
                  apiKey_;
            hdrs = {"Content-Type: application/json"};
            break;

        case AiProv::ANTHROPIC:
            body = antBody(systemPrompt, userMsg);
            url = "https://api.anthropic.com/v1/messages";
            hdrs = {"x-api-key: " + apiKey_,
                    "anthropic-version: 2023-06-01",
                    "Content-Type: application/json"};
            break;

        case AiProv::OPENAI:
            body = oaiBody(systemPrompt, userMsg, "gpt-3.5-turbo");
            url = "https://api.openai.com/v1/chat/completions";
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            break;

        case AiProv::CUSTOM:
            body = oaiBody(systemPrompt, userMsg, "default");
            url = customUrl_;
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            break;

        default:
            return mock(userMsg);
        }

        resp = Http::post(url, body, hdrs, outErr);

        if (resp.empty())
        {
            if (outErr.empty())
                outErr = "Empty response from server.";
            return "[ERR] " + outErr;
        }
        if (Json::hasError(resp))
        {
            outErr = "API returned an error: " + resp.substr(0, 200);
            return "[ERR] API error. Check your key. " + Json::str(resp, "message");
        }
        return Json::firstText(resp);
    }
};

// ============================================================================
//  LOGGER
// ============================================================================
class Log
{
    std::string path_ = "nexus_log.txt";
    std::string ts()
    {
        time_t t = time(nullptr);
        char b[32];
        strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return b;
    }

public:
    void log(const std::string &user, const std::string &action,
             const std::string &detail = "", const std::string &level = "INFO")
    {
        std::ofstream f(path_, std::ios::app);
        if (f)
            f << "[" << ts() << "] [" << level << "] user=" << user
              << " action=" << action << (detail.empty() ? "" : " " + detail) << "\n";
    }
    std::vector<std::string> recent(int n = 20)
    {
        std::vector<std::string> lines, all;
        std::ifstream f(path_);
        std::string s;
        while (std::getline(f, s))
            all.push_back(s);
        int start = std::max(0, (int)all.size() - n);
        for (int i = start; i < (int)all.size(); ++i)
            lines.push_back(all[i]);
        return lines;
    }
};

// ============================================================================
//  USER & ROLE
// ============================================================================
enum class Role
{
    USER,
    ADMIN
};

struct User
{
    std::string uname, passHash, displayName, email;
    Role role;
    std::string joinDate;

    User() = default;
    User(const std::string &u, const std::string &ph,
         const std::string &dn, const std::string &em,
         Role r, const std::string &jd)
        : uname(u), passHash(ph), displayName(dn), email(em), role(r), joinDate(jd) {}
};

// ============================================================================
//  SIMPLE HASH  (djb2 -- not cryptographic, mirrors original style)
// ============================================================================
static std::string hashPass(const std::string &p)
{
    unsigned long h = 5381;
    for (char c : p)
        h = ((h << 5) + h) + (unsigned char)c;
    std::ostringstream os;
    os << std::hex << std::setw(16) << std::setfill('0') << h;
    return os.str();
}

// ============================================================================
//  USER STORE  (file persistence)
// ============================================================================
class UserStore
{
    static std::string path_() { return "nexus_users.txt"; }

    static std::string roleStr(Role r) { return r == Role::ADMIN ? "ADMIN" : "USER"; }
    static Role parseRole(const std::string &s) { return s == "ADMIN" ? Role::ADMIN : Role::USER; }

    // Field delimiter that won't appear in normal strings
    static constexpr char SEP = '|';

public:
    static std::string path() { return path_(); }

    static void save(const std::vector<User> &users)
    {
        std::ofstream f(path_());
        if (!f)
            return;
        for (auto &u : users)
        {
            f << u.uname << SEP
              << u.passHash << SEP
              << u.displayName << SEP
              << u.email << SEP
              << roleStr(u.role) << SEP
              << u.joinDate << "\n";
        }
    }

    static std::vector<User> load()
    {
        std::vector<User> users;
        std::ifstream f(path_());
        if (!f)
            return users;
        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty())
                continue;
            std::vector<std::string> fld;
            std::istringstream ss(line);
            std::string token;
            while (std::getline(ss, token, SEP))
                fld.push_back(token);
            if (fld.size() < 6)
                continue;
            users.emplace_back(fld[0], fld[1], fld[2], fld[3],
                               parseRole(fld[4]), fld[5]);
        }
        return users;
    }

    static User *find(std::vector<User> &users, const std::string &uname)
    {
        for (auto &u : users)
            if (u.uname == uname)
                return &u;
        return nullptr;
    }
};

// ============================================================================
//  CONFIG STORE  (persists API key between runs)
// ============================================================================
static std::string gAiKey, gCustomUrl;

class ConfigStore
{
    static std::string path_() { return "nexus_config.txt"; }

public:
    static std::string path() { return path_(); }

    static void save()
    {
        std::ofstream f(path_());
        if (f)
        {
            f << "ai_key=" << gAiKey << "\n";
            f << "custom_url=" << gCustomUrl << "\n";
        }
    }

    static void load()
    {
        std::ifstream f(path_());
        if (!f)
            return;
        std::string line;
        while (std::getline(f, line))
        {
            if (line.substr(0, 7) == "ai_key=")
                gAiKey = line.substr(7);
            if (line.substr(0, 11) == "custom_url=")
                gCustomUrl = line.substr(11);
        }
    }
};

// ============================================================================
//  CHAT HISTORY  (per-user file)
// ============================================================================
struct ChatMsg
{
    std::string ts, role, text;
};

class ChatHistory
{
    std::string path_;
    std::string ts_()
    {
        time_t t = time(nullptr);
        char b[32];
        strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return b;
    }

public:
    explicit ChatHistory(const std::string &uname)
        : path_("history_" + uname + ".txt") {}

    void append(const std::string &role, const std::string &text)
    {
        std::ofstream f(path_, std::ios::app);
        if (f)
            f << ts_() << "|" << role << "|" << text << "\n";
    }

    std::vector<ChatMsg> load(int maxLines = 50)
    {
        std::vector<ChatMsg> all, result;
        std::ifstream f(path_);
        if (!f)
            return result;
        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty())
                continue;
            auto p1 = line.find('|');
            auto p2 = (p1 != std::string::npos) ? line.find('|', p1 + 1) : std::string::npos;
            if (p2 == std::string::npos)
                continue;
            ChatMsg m;
            m.ts = line.substr(0, p1);
            m.role = line.substr(p1 + 1, p2 - p1 - 1);
            m.text = line.substr(p2 + 1);
            all.push_back(m);
        }
        int start = std::max(0, (int)all.size() - maxLines);
        for (int i = start; i < (int)all.size(); ++i)
            result.push_back(all[i]);
        return result;
    }

    void clear()
    {
        std::ofstream f(path_, std::ios::trunc);
    }

    const std::string &filePath() const { return path_; }
};

// ============================================================================
//  FORWARD DECLARATIONS
// ============================================================================
static Chatbot gBot;
static Log gLog;
static const std::string ADMIN_REG_KEY = "NEXUS-ADMIN-2025";

// ============================================================================
//  CHAT SESSION
// ============================================================================
class ChatSession
{
    User &user_;
    ChatHistory hist_;

    std::string systemPrompt()
    {
        return "You are NexusChat, a friendly and knowledgeable AI assistant. "
               "You are speaking with " +
               user_.displayName + ". "
                                   "Be concise, helpful, accurate, and professional. "
                                   "If asked about your identity, say you are NexusChat powered by " +
               gBot.provName() + ".";
    }

    void printMsgBox(const std::string &role, const std::string &text,
                     const std::string &ts = "")
    {
        bool isBot = (role == "BOT");
        std::string col = isBot ? C::BCYN : C::BYLW;
        std::string lbl = isBot ? "  NexusChat" : "  You       ";
        std::cout << "\n"
                  << col << C::B << lbl;
        if (!ts.empty())
            std::cout << C::DIM << "  " << ts;
        std::cout << C::R << "\n";
        UI::hline('-', 76, isBot ? C::CYN : C::YLW);
        UI::typeOut(text);
        UI::hline('-', 76, C::DIM);
    }

public:
    explicit ChatSession(User &u) : user_(u), hist_(u.uname) {}

    void run()
    {
        UI::clearScreen();
        UI::title("NEXUSCHAT  --  Live AI Chat", C::BCYN);
        UI::kv("Logged in as", user_.displayName, C::YLW, C::BWHT);
        UI::kv("AI Provider", gBot.provName(),
               C::YLW, gBot.isMock() ? C::BYLW : C::BGRN);
        if (gBot.isMock())
            UI::wrn("DEMO mode -- set API key in Settings for real AI responses.");
        else
            UI::ok("Live AI connected.");

        UI::inf("Type your message and press ENTER. Commands: /history  /clear  /exit");
        UI::hline('=', 76, C::BCYN);

        // Show last 5 messages from history on session start
        auto prev = hist_.load(5);
        if (!prev.empty())
        {
            UI::inf("-- Resuming previous conversation --");
            for (auto &m : prev)
                printMsgBox(m.role, m.text, m.ts);
        }

        std::string sys = systemPrompt();

        while (true)
        {
            std::cout << "\n"
                      << C::BYLW << C::B << "  >> You" << C::R
                      << C::YLW << " : " << C::R;
            std::string msg;
            std::getline(std::cin, msg);

            if (msg.empty())
                continue;
            if (msg == "/exit" || msg == "/quit")
                break;

            if (msg == "/history")
            {
                UI::section("CHAT HISTORY (last 20)", C::BCYN);
                auto lines = hist_.load(20);
                if (lines.empty())
                {
                    UI::inf("No history yet.");
                    continue;
                }
                for (auto &m : lines)
                    printMsgBox(m.role, m.text, m.ts);
                continue;
            }
            if (msg == "/clear")
            {
                hist_.clear();
                UI::ok("Chat history cleared.");
                gLog.log(user_.uname, "HISTORY_CLEAR");
                continue;
            }

            hist_.append("YOU", msg);
            gLog.log(user_.uname, "CHAT_MSG", "len=" + std::to_string(msg.size()));

            UI::spinner("NexusChat is thinking...");

            std::string err;
            std::string reply = gBot.ask(msg, sys, err);

            if (!err.empty() && reply.substr(0, 5) == "[ERR]")
                UI::err("AI error: " + err);

            hist_.append("BOT", reply);
            printMsgBox("BOT", reply);
        }
        UI::ok("Chat session ended.");
        gLog.log(user_.uname, "CHAT_END");
    }
};

// ============================================================================
//  PROFILE VIEW
// ============================================================================
void showProfile(const User &u)
{
    UI::clearScreen();
    UI::title("MY PROFILE", C::BMAG);
    UI::section("Account Details", C::BMAG);
    UI::kv("Display Name", u.displayName);
    UI::kv("Username", u.uname);
    UI::kv("Email", u.email);
    UI::kv("Role", u.role == Role::ADMIN ? "Administrator" : "User");
    UI::kv("Joined", u.joinDate);
    UI::kv("History File", "history_" + u.uname + ".txt");
    UI::pause();
}

// ============================================================================
//  SETTINGS (configure AI key)
// ============================================================================
void settingsMenu(User &user, std::vector<User> &users)
{
    while (true)
    {
        UI::clearScreen();
        UI::title("SETTINGS", C::BMAG);

        UI::section("AI Configuration", C::BMAG);
        UI::kv("Current Provider", gBot.provName(),
               C::YLW, gBot.isMock() ? C::BYLW : C::BGRN);
        UI::kv("HTTP Backend",
               Http::backendAvailable() ? "Available" : "NOT available",
               C::YLW, Http::backendAvailable() ? C::BGRN : C::BRED);

        UI::menuBox("Settings Menu",
                    {{"1", "Set AI API Key"},
                     {"2", "Set Custom AI Endpoint URL (for custom provider)"},
                     {"3", "Test AI Connection"},
                     {"4", "Change Display Name"},
                     {"5", "Change Email"},
                     {"0", "Back"}},
                    C::BMAG);

        int ch = UI::menu(0, 5);
        if (ch == 0)
            break;

        if (ch == 1)
        {
            UI::section("Set AI API Key", C::BMAG);
            UI::inf("Prefix detection:  gsk_... = Groq  |  AIza... = Gemini");
            UI::inf("                   sk-ant-... = Anthropic  |  sk-... = OpenAI");
            std::string key = UI::input("Paste API Key (blank = keep current)");
            if (!key.empty())
            {
                gAiKey = key;
                gBot.configure(gAiKey, gCustomUrl);
                ConfigStore::save();
                UI::ok("Key saved. Provider detected: " + gBot.provName());
                gLog.log(user.uname, "API_KEY_SET", "provider=" + gBot.provName());
            }
            else
            {
                UI::wrn("No change.");
            }
            UI::pause();
        }
        else if (ch == 2)
        {
            std::string url = UI::input("Custom endpoint URL (e.g. https://myserver/v1/chat/completions)");
            if (!url.empty())
            {
                gCustomUrl = url;
                gBot.configure(gAiKey, gCustomUrl);
                ConfigStore::save();
                UI::ok("Custom URL saved.");
                gLog.log(user.uname, "CUSTOM_URL_SET");
            }
            UI::pause();
        }
        else if (ch == 3)
        {
            UI::section("Connection Test", C::BMAG);
            if (!Http::backendAvailable())
            {
                UI::err("HTTP backend is NOT available on this build.");
                UI::pause();
                continue;
            }
            if (gBot.isMock())
            {
                UI::wrn("No AI key configured -- running in DEMO mode.");
                UI::pause();
                continue;
            }
            UI::spinner("Sending test ping to " + gBot.provName() + "...");
            std::string err;
            std::string reply = gBot.ask("Say only: NexusChat connection OK",
                                         "You are a test bot. Reply exactly as asked.",
                                         err);
            if (err.empty() && reply.find("[ERR]") == std::string::npos)
            {
                UI::ok("Connection successful!");
                std::cout << C::BGRN << "  Reply: " << C::R << reply << "\n";
                gLog.log(user.uname, "CONN_TEST", "OK");
            }
            else
            {
                UI::err("Connection failed: " + err);
                gLog.log(user.uname, "CONN_TEST", "FAIL " + err, "ERROR");
            }
            UI::pause();
        }
        else if (ch == 4)
        {
            std::string dn = UI::input("New Display Name");
            if (!dn.empty())
            {
                user.displayName = dn;
                UserStore::save(users);
                UI::ok("Display name updated.");
                gLog.log(user.uname, "PROFILE_UPDATE", "displayName");
            }
            UI::pause();
        }
        else if (ch == 5)
        {
            std::string em = UI::input("New Email");
            if (!em.empty())
            {
                user.email = em;
                UserStore::save(users);
                UI::ok("Email updated.");
                gLog.log(user.uname, "PROFILE_UPDATE", "email");
            }
            UI::pause();
        }
    }
}

// ============================================================================
//  USER DASHBOARD
// ============================================================================
class UserDash
{
    User &user_;
    std::vector<User> &users_;

public:
    UserDash(User &u, std::vector<User> &all) : user_(u), users_(all) {}

    void run()
    {
        while (true)
        {
            UI::clearScreen();
            UI::title("USER PORTAL  --  " + user_.displayName, C::BCYN);
            UI::kv("AI Provider", gBot.provName(),
                   C::YLW, gBot.isMock() ? C::BYLW : C::BGRN);

            UI::menuBox("Main Menu",
                        {{"1", "Start Chat"},
                         {"2", "View Chat History"},
                         {"3", "My Profile"},
                         {"4", "Settings / API Key"},
                         {"0", "Logout"}},
                        C::BCYN);

            int ch = UI::menu(0, 4);
            if (ch == 0)
                break;

            if (ch == 1)
            {
                ChatSession sess(user_);
                sess.run();
            }
            else if (ch == 2)
            {
                UI::clearScreen();
                UI::title("CHAT HISTORY", C::BCYN);
                ChatHistory hist(user_.uname);
                auto lines = hist.load(30);
                if (lines.empty())
                {
                    UI::inf("No history found.");
                }
                else
                {
                    for (auto &m : lines)
                    {
                        bool isBot = (m.role == "BOT");
                        std::string col = isBot ? C::BCYN : C::BYLW;
                        std::cout << col << C::B << "  [" << m.ts << "] "
                                  << (isBot ? "NexusChat" : "You")
                                  << C::R << "\n";
                        UI::typeOut(m.text);
                    }
                }
                UI::pause();
            }
            else if (ch == 3)
            {
                showProfile(user_);
            }
            else if (ch == 4)
            {
                settingsMenu(user_, users_);
                // Re-apply key after settings change
                gBot.configure(gAiKey, gCustomUrl);
            }
        }
        gLog.log(user_.uname, "LOGOUT");
    }
};

// ============================================================================
//  ADMIN DASHBOARD
// ============================================================================
class AdminDash
{
    User &user_;
    std::vector<User> &users_;

    void listUsers()
    {
        UI::clearScreen();
        UI::title("USER MANAGEMENT", C::BRED);
        UI::section("All Registered Accounts", C::BRED);
        int idx = 1;
        for (auto &u : users_)
        {
            std::string roleBadge = (u.role == Role::ADMIN)
                                        ? UI::badge("ADMIN", false)
                                        : UI::badge(" USER", true);
            std::cout << C::BYLW << "  " << std::setw(3) << idx++ << ". "
                      << C::BWHT << std::setw(16) << std::left << u.uname
                      << roleBadge << "  "
                      << C::CYN << u.displayName << C::DIM << "  <" << u.email << ">"
                      << C::R << "\n";
        }
        UI::pause();
    }

    void deleteUser()
    {
        UI::section("Delete User", C::BRED);
        std::string uname = UI::input("Username to delete");
        if (uname == user_.uname)
        {
            UI::err("Cannot delete your own account.");
            UI::pause();
            return;
        }
        auto it = std::find_if(users_.begin(), users_.end(),
                               [&](const User &u)
                               { return u.uname == uname; });
        if (it == users_.end())
        {
            UI::err("User not found.");
            UI::pause();
            return;
        }
        users_.erase(it);
        UserStore::save(users_);
        UI::ok("User '" + uname + "' deleted.");
        gLog.log(user_.uname, "DELETE_USER", "target=" + uname, "ADMIN");
        UI::pause();
    }

    void viewLogs()
    {
        UI::clearScreen();
        UI::title("SYSTEM LOGS (last 25)", C::BRED);
        auto lines = gLog.recent(25);
        if (lines.empty())
        {
            UI::inf("No log entries yet.");
        }
        else
        {
            for (auto &l : lines)
                std::cout << C::DIM << "  " << l << C::R << "\n";
        }
        UI::pause();
    }

    void sysInfo()
    {
        UI::clearScreen();
        UI::title("SYSTEM INFORMATION", C::BRED);
        UI::section("Build & Runtime", C::BRED);
        UI::kv("Application", "NexusChat v1.0");
        UI::kv("Platform", PLATFORM_WINDOWS ? "Windows (WinHTTP)" : "Linux/macOS");
        UI::kv("HTTP Backend", Http::backendAvailable() ? "Available" : "NOT available");
    }

    void globalApiKey()
    {
        UI::section("Configure Global AI API Key", C::BRED);
        UI::inf("This sets the AI key for ALL users on this instance.");
        std::string key = UI::input("Paste API Key");
        if (!key.empty())
        {
            gAiKey = key;
            gBot.configure(gAiKey, gCustomUrl);
            ConfigStore::save();
            UI::ok("Global key saved. Provider: " + gBot.provName());
            gLog.log(user_.uname, "GLOBAL_KEY_SET", "provider=" + gBot.provName(), "ADMIN");
        }
        else
        {
            UI::wrn("No change.");
        }
        UI::pause();
    }

public:
    AdminDash(User &u, std::vector<User> &all) : user_(u), users_(all) {}

    void run()
    {
        while (true)
        {
            UI::clearScreen();
            UI::title("ADMIN PANEL  --  " + user_.displayName, C::BRED);
            UI::kv("AI Provider", gBot.provName(),
                   C::YLW, gBot.isMock() ? C::BYLW : C::BGRN);
            UI::kv("HTTP Backend",
                   Http::backendAvailable() ? "OK" : "UNAVAILABLE",
                   C::YLW, Http::backendAvailable() ? C::BGRN : C::BRED);

            UI::menuBox("Admin Menu",
                        {{"1", "Start Chat (own session)"},
                         {"2", "List All Users"},
                         {"3", "Delete a User"},
                         {"4", "View System Logs"},
                         {"5", "System Information"},
                         {"6", "Configure Global AI Key"},
                         {"7", "Settings / My Profile"},
                         {"0", "Logout"}},
                        C::BRED);

            int ch = UI::menu(0, 7);
            if (ch == 0)
                break;

            switch (ch)
            {
            case 1:
            {
                ChatSession sess(user_);
                sess.run();
                break;
            }
            case 2:
                listUsers();
                break;
            case 3:
                deleteUser();
                break;
            case 4:
                viewLogs();
                break;
            case 5:
                sysInfo();
                break;
            case 6:
                globalApiKey();
                break;
            case 7:
                settingsMenu(user_, users_);
                gBot.configure(gAiKey, gCustomUrl);
                break;
            }
        }
        gLog.log(user_.uname, "ADMIN_LOGOUT");
    }
};

// ============================================================================
//  SIGN-UP
// ============================================================================
static void doSignup(std::vector<User> &users, Role role)
{
    UI::clearScreen();
    std::string roleLabel = (role == Role::ADMIN) ? "Administrator" : "User";
    UI::title("REGISTER -- " + roleLabel, C::BGRN);

    if (role == Role::ADMIN)
    {
        UI::section("Admin Key Required", C::BRED);
        std::string key = UI::password("Admin Registration Key");
        if (key != ADMIN_REG_KEY)
        {
            UI::err("Invalid admin key. Registration denied.");
            gLog.log("?", "SIGNUP_DENIED", "role=ADMIN", "WARNING");
            UI::pause();
            return;
        }
        UI::ok("Admin key accepted.");
    }

    UI::section("Account Details", C::BGRN);
    std::string uname = UI::input("Choose Username");
    if (uname.empty() || uname.size() < 3)
    {
        UI::err("Username must be at least 3 characters.");
        UI::pause();
        return;
    }
    if (UserStore::find(users, uname))
    {
        UI::err("Username already taken.");
        UI::pause();
        return;
    }

    std::string pass1 = UI::password("Choose Password (min 6 chars)");
    if (pass1.size() < 6)
    {
        UI::err("Password too short (min 6 characters).");
        UI::pause();
        return;
    }
    std::string pass2 = UI::password("Confirm Password");
    if (pass1 != pass2)
    {
        UI::err("Passwords do not match.");
        UI::pause();
        return;
    }

    std::string dname = UI::input("Display Name");
    if (dname.empty())
        dname = uname;

    std::string email = UI::input("Email Address");

    // Build join date
    time_t t = time(nullptr);
    char dateBuf[32];
    strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", localtime(&t));

    users.emplace_back(uname, hashPass(pass1), dname, email, role, std::string(dateBuf));
    UserStore::save(users);

    UI::ok("Account created! Welcome, " + dname + ".");
    gLog.log(uname, "SIGNUP", "role=" + roleLabel);
    UI::pause();
}

// ============================================================================
//  LOGIN
// ============================================================================
static User *doLogin(std::vector<User> &users, Role requiredRole)
{
    UI::clearScreen();
    std::string roleLabel = (requiredRole == Role::ADMIN) ? "Administrator" : "User";
    UI::title("LOGIN -- " + roleLabel, C::BCYN);

    static const int MAX_ATTEMPTS = 3;
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt)
    {
        UI::section("Attempt " + std::to_string(attempt) + " / " + std::to_string(MAX_ATTEMPTS),
                    C::BCYN);
        std::string uname = UI::input("Username");
        std::string pass = UI::password("Password");

        User *u = UserStore::find(users, uname);
        if (!u || u->passHash != hashPass(pass))
        {
            UI::err("Invalid username or password.");
            gLog.log(uname, "LOGIN_FAIL", "attempt=" + std::to_string(attempt), "WARNING");
            continue;
        }
        if (u->role != requiredRole)
        {
            UI::err("Account role mismatch. Access denied.");
            gLog.log(uname, "LOGIN_ROLE_MISMATCH", "required=" + roleLabel, "WARNING");
            UI::pause();
            return nullptr;
        }
        UI::ok("Welcome back, " + u->displayName + "!");
        gLog.log(uname, "LOGIN_OK", "role=" + roleLabel);
        UI::pause();
        return u;
    }
    UI::err("Too many failed attempts. Account temporarily locked.");
    gLog.log("?", "LOGIN_LOCKED", "role=" + roleLabel, "CRITICAL");
    UI::pause();
    return nullptr;
}

// ============================================================================
//  MAIN APPLICATION
// ============================================================================
class App
{
    std::vector<User> users_;

    void seed()
    {
        if (!users_.empty())
            return;
        time_t t = time(nullptr);
        char dateBuf[32];
        strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", localtime(&t));
        std::string today = dateBuf;
        users_.emplace_back("admin", hashPass("Admin@123"), "System Administrator",
                            "admin@nexus.ai", Role::ADMIN, today);
        users_.emplace_back("user1", hashPass("User@123"), "Alex",
                            "alex@nexus.ai", Role::USER, today);
        UserStore::save(users_);
        UI::inf("Default accounts seeded to " + UserStore::path());
    }

    void splash()
    {
        UI::clearScreen();
        // NexusChat ASCII art banner
        std::cout << C::BCYN << C::B;
        std::cout << "\n";
        std::cout << "  ███╗   ██╗███████╗██╗  ██╗██╗   ██╗███████╗\n";
        std::cout << "  ████╗  ██║██╔════╝╚██╗██╔╝██║   ██║██╔════╝\n";
        std::cout << "  ██╔██╗ ██║█████╗   ╚███╔╝ ██║   ██║███████╗\n";
        std::cout << "  ██║╚██╗██║██╔══╝   ██╔██╗ ██║   ██║╚════██║\n";
        std::cout << "  ██║ ╚████║███████╗██╔╝ ██╗╚██████╔╝███████║\n";
        std::cout << "  ╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝\n";
        std::cout << C::R;
        std::cout << C::BBLU << C::B;
        std::cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        std::cout << "   ~  ~  ~  C H A T  A I  A S S I S T A N T  ~  ~\n";
        std::cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        std::cout << C::R;
        std::cout << C::BCYN << C::B;
        std::cout << "  ====================================================\n";
        std::cout << "   NexusChat v1.0  --  Universal AI Chatbot Platform\n";
        std::cout << "  ====================================================\n";
        std::cout << C::R;
        std::cout << C::DIM;
        std::cout << "   AI Support   : Groq | Gemini | Anthropic | OpenAI | Custom\n";
        std::cout << "   Key Detect   : paste any key -- provider auto-detected\n";
        std::cout << "   Auth         : file-based login (nexus_users.txt)\n";
        std::cout << "   History      : per-user chat logs (history_<user>.txt)\n";
        std::cout << "   Config       : keys persist across restarts (nexus_config.txt)\n";
        std::cout << "  ====================================================\n";
        std::cout << C::R << "\n";

        // HTTP backend check
        if (Http::backendAvailable())
            UI::ok("HTTP backend: " + std::string(
#if PLATFORM_WINDOWS
                                          "WinHTTP (Windows)"
#else
                                          "none"
#endif
                                          ));
        else
            UI::err("HTTP backend NOT available -- use Windows build.");

        // AI status
        if (gBot.isMock())
            UI::wrn("AI: DEMO mode (set key via Settings or Admin panel)");
        else
            UI::ok("AI: LIVE -- " + gBot.provName());

        std::cout << "\n  Users file  : " << C::CYN << UserStore::path() << C::R << "\n";
        std::cout << "  Config file : " << C::CYN << ConfigStore::path() << C::R << "\n\n";

        gLog.log("SYSTEM", "STARTUP", "NexusChat v1.0");
    }

public:
    App()
    {
        users_ = UserStore::load();
        seed();
        ConfigStore::load();
        gBot.configure(gAiKey, gCustomUrl);
    }

    void run()
    {
        splash();
        UI::pause();

        while (true)
        {
            UI::clearScreen();
            UI::title("NEXUSCHAT AI CHATBOT  v1.0", C::BCYN);

            UI::menuBox("MAIN MENU  --  Select Action",
                        {{"1", "Login  as User"},
                         {"2", "Login  as Administrator"},
                         {"3", "Register as User         (free)"},
                         {"4", "Register as Administrator (key: " + ADMIN_REG_KEY + ")"},
                         {"0", "Exit"}},
                        C::BCYN);

            std::cout << "\n"
                      << C::DIM
                      << "  Demo credentials:  user1 / User@123    admin / Admin@123\n"
                      << C::R;

            int ch = UI::menu(0, 4);
            if (ch == 0)
                break;

            if (ch == 1 || ch == 2)
            {
                Role r = (ch == 1) ? Role::USER : Role::ADMIN;
                User *u = doLogin(users_, r);
                if (!u)
                    continue;

                if (u->role == Role::ADMIN)
                {
                    AdminDash dash(*u, users_);
                    dash.run();
                }
                else
                {
                    UserDash dash(*u, users_);
                    dash.run();
                }

                UI::clearScreen();
                std::cout << C::BCYN << C::B << "\n  You have been logged out. Goodbye!\n\n"
                          << C::R;
                UI::pause();
            }
            else
            {
                Role r = (ch == 3) ? Role::USER : Role::ADMIN;
                doSignup(users_, r);
            }
        }

        UI::clearScreen();
        std::cout << C::BCYN << C::B << R"(
  ============================================
   Thank you for using NexusChat v1.0!
   Stay connected. Stay smart.
  ============================================
)" << C::R;
        gLog.log("SYSTEM", "SHUTDOWN", "clean exit");
    }
};

// ============================================================================
//  ENTRY POINT
// ============================================================================
int main()
{
#ifdef _WIN32
    // Windows console setup (requires Windows SDK)
    // Stubbed when winhttp.h is not available
    SetConsoleOutputCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE)
    {
        DWORD m = 0;
        GetConsoleMode(h, &m);
        #ifdef ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        #endif
    }
#endif
    try
    {
        App app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nFATAL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}