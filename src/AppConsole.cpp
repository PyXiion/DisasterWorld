#include <Disaster/AppConsole.hpp>
#include <algorithm>
#include <iostream>
#include <cstring>

#include <Disaster/Program.hpp>
#include <angelscript/scripthelper.h>
#include <easy/profiler.h>

std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
      token = s.substr (pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

namespace px::disaster {
  AppConsole *AppConsole::instance = nullptr;

  AppConsole::AppConsole() {
    instance = this;
    std::memset(m_inputBuf, 0, sizeof(m_inputBuf));
    m_autoScroll = true;
    m_open = true;

    m_title = "Console";
    m_commands = {
      "help", "exec", "eval", "addVar", "delVar", "addFunc", "delFunc", "listVars", "listFuncs",
    };
    m_historyPos = -1;

    m_scriptEngine = Program::GetInstance().GetScriptEngine().Get();

    ConfigureEngine();
  }

  void AppConsole::ConfigureEngine() {
    EASY_FUNCTION();
    int r;
    // Eval printing
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(bool)", asMETHODPR(AppConsole, _Grab, (bool), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(int)", asMETHODPR(AppConsole, _Grab, (int), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(uint)", asMETHODPR(AppConsole, _Grab, (unsigned int), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(float)", asMETHODPR(AppConsole, _Grab, (float), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(double)", asMETHODPR(AppConsole, _Grab, (double), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    r = m_scriptEngine->RegisterGlobalFunction("void _grab()", asMETHODPR(AppConsole, _Grab, (void), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = m_scriptEngine->RegisterGlobalFunction("void _grab(const string &in)", asMETHODPR(AppConsole, _Grab, (const std::string &), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    // Console API
    r = m_scriptEngine->RegisterObjectType("CConsole", 0, asOBJ_REF | asOBJ_NOHANDLE); assert(r >= 0);

    r = m_scriptEngine->RegisterObjectMethod("CConsole", "void clear()", asMETHOD(AppConsole, ClearLog), asCALL_THISCALL); assert(r >= 0);
    r = m_scriptEngine->RegisterObjectMethod("CConsole", "void log(const string &in)", asMETHOD(AppConsole, Log), asCALL_THISCALL); assert(r >= 0);
    r = m_scriptEngine->RegisterObjectMethod("CConsole", "void warn(const string &in)", asMETHOD(AppConsole, Warn), asCALL_THISCALL); assert(r >= 0);
    r = m_scriptEngine->RegisterObjectMethod("CConsole", "void error(const string &in)", asMETHOD(AppConsole, Error), asCALL_THISCALL); assert(r >= 0);

    r = m_scriptEngine->RegisterGlobalProperty("CConsole console", this); assert(r >= 0);
  }

  void AppConsole::AddLog(const char *fmt, ...) {
    std::lock_guard<std::mutex> lk(m_mutex);
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    std::cout << buf << std::endl;
    buf[sizeof(buf)-1] = 0;
    va_end(args);
    m_items.emplace_back(buf, sizeof(buf));
  }
  void AppConsole::ClearLog() {
    m_items.clear();
  }

  void AppConsole::Log(const std::string &message) {
    AddLog("%s", message.c_str());
  }
  void AppConsole::Warn(const std::string &message) {
    AddLog("[warning] %s", message.c_str());
  }
  void AppConsole::Error(const std::string &message) {
    AddLog("[error] %s", message.c_str());
  }

  void AppConsole::Draw() {
    EASY_FUNCTION();
    if (!m_open) return;
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(m_title.c_str(), &m_open)) {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Clear")) ClearLog();
    ImGui::SameLine();

    bool copyToClipboard = ImGui::Button("Copy");
    ImGui::SameLine();

    // Options menu
    if (ImGui::BeginPopup("Options")) {
      ImGui::Checkbox("Auto-scroll", &m_autoScroll);
      ImGui::EndPopup();
    }
    // History list
    if (ImGui::BeginPopup("History")) {
      for (auto it = m_history.rbegin(); it != m_history.rend(); ++it) {
        if (ImGui::Selectable(it->c_str())) {
          std::strncpy(m_inputBuf, it->c_str(), sizeof(m_inputBuf));
          m_historyPos = m_history.size() - 1 - (it - m_history.rbegin());
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndPopup();
    }

    // Options
    if (ImGui::Button("Options"))
      ImGui::OpenPopup("Options");
    ImGui::SameLine();
    // History
    if (ImGui::Button("History"))
      ImGui::OpenPopup("History");
      
    m_filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
      if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear")) ClearLog();
        ImGui::EndPopup();
      }

      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
      if (copyToClipboard)
        ImGui::LogToClipboard();

      for (const auto &item : m_items) {
        if (!m_filter.PassFilter(item.c_str()))
          continue;
        
        ImVec4 color;
        bool hasColor = false;
        if (item.starts_with("[error]")) { color = ImVec4(0.8f, 0.2f, 0.0f, 1.0f); hasColor = true; }
        else if (item.starts_with("[warning]")) { color = ImVec4(1.0f, 0.6f, 0.4f, 1.0f); hasColor = true; }
        else if (item.starts_with("#")) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); hasColor = true; }
        if (hasColor)
          ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item.c_str());
        if (hasColor)
          ImGui::PopStyleColor();
      }
      if (copyToClipboard)
        ImGui::LogFinish();

      if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
      
      ImGui::PopStyleVar();

      ImGui::EndChild();
      ImGui::Separator();

      // Command-line
      bool reclaimFocus = false;
      ImGuiInputTextFlags inputTextFlags = 0;
      inputTextFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
      inputTextFlags |= ImGuiInputTextFlags_EscapeClearsAll;
      inputTextFlags |= ImGuiInputTextFlags_CallbackCompletion;
      inputTextFlags |= ImGuiInputTextFlags_CallbackHistory;

      static ImGuiInputTextCallback stubCallback = [](ImGuiInputTextCallbackData *data) -> int {
        AppConsole *console = (AppConsole*) data->UserData;
        return console->TextEditCallback(data);
      };

      if (ImGui::InputText("Input", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), inputTextFlags, stubCallback, (void*) this)) {
        char *s = m_inputBuf;
        // add trim later
        if (s[0])
          Execute(std::string(s));
        strcpy(s, "");
        reclaimFocus = true;
      }

      // Auto-focus on window apparition
      ImGui::SetItemDefaultFocus();
      if (reclaimFocus)
        ImGui::SetKeyboardFocusHere(-1);
      
      ImGui::End();
    }
  }

  int AppConsole::TextEditCallback(ImGuiInputTextCallbackData *data) {
    switch (data->EventFlag) {
      case ImGuiInputTextFlags_CallbackCompletion: {
        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf)
        {
          const char c = word_start[-1];
          if (c == ' ' || c == '\t' || c == ',' || c == ';')
            break;
          word_start--;
        }

        std::string word(word_start, word_end - word_start);

        // Build a list of candidates
        std::vector<std::string> candidates;
        for (const auto &elem : m_commands)
          if (elem.starts_with(word))
            candidates.push_back(elem);

        if (candidates.size() == 0) {
          // No match
          AddLog("No match for \"%.*s\"!", (int)(word_end - word_start), word_start);
        }
        else if (candidates.size() == 1) {
            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
          data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
          data->InsertChars(data->CursorPos, candidates[0].c_str());
          data->InsertChars(data->CursorPos, " ");
        }
        else {
          // Multiple matches. Complete as much as we can..
          // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
          int match_len = (int)(word_end - word_start);
          for (;;) {
            int c = 0;
            bool all_candidates_matches = true;
            for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
              if (i == 0)
                c = toupper(candidates[i][match_len]);
              else if (c == 0 || c != toupper(candidates[i][match_len]))
                all_candidates_matches = false;
            if (!all_candidates_matches)
              break;
            match_len++;
          }

          if (match_len > 0)
          {
            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0].c_str(), candidates[0].c_str() + match_len);
          }

          // List matches
          AddLog("Possible matches:");
          for (int i = 0; i < candidates.size(); i++)
              AddLog("- %s", candidates[i].c_str());
        }
        break;
      }
      case ImGuiInputTextFlags_CallbackHistory: {
        const int prevHistoryPos = m_historyPos;
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (m_historyPos == -1)
            m_historyPos = m_history.size() - 1;
          else if (m_historyPos > 0)
            m_historyPos--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow) {
          if (m_historyPos != -1)
            if (++m_historyPos >= m_history.size())
              m_historyPos = -1;
        }

        // Preserve the data on the current input
        std::string commandLine(data->Buf, data->BufTextLen);
        if (prevHistoryPos == -1 && !(m_history.size() > 1 && m_history.back() == commandLine) && !commandLine.empty())
          m_history.push_back(commandLine);

        if (prevHistoryPos != m_historyPos)
        {
          std::string historyStr = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, historyStr.c_str());
        }
      }
    }
    return 0;
  }

  void AppConsole::Execute(const std::string &commandLine) {
    AddLog("# %s", commandLine.c_str());

    // Insert into history if not repeated.
    m_historyPos = -1;
    if (!(m_history.size() > 1 && m_history.back() == commandLine))
      m_history.push_back(commandLine);

    std::string cmd, arg;
    size_t pos;
		if( (pos = commandLine.find(" ")) != std::string::npos ) {
			cmd = commandLine.substr(0, pos);
			arg = commandLine.substr(pos+1);
		}
		else {
			cmd = commandLine;
			arg = "";
		}
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
    [](unsigned char c){ return std::tolower(c); });
    
    if (cmd == "help") {
      for (const auto &cmd : m_commands)
        AddLog("- %s", cmd.c_str());
      return;
    }
    else if (cmd == "clear") {
      ClearLog();
      return;
    }
    // AngelScript
#define APP_CONSOLE_AS_COMMAND(commandName, functionCall, usage)                                      \
    else if (cmd == commandName) {                                                                    \
      if (arg.empty()) {                                                                              \
        AddLog("[error] Command " commandName " requires arguments\nUsage: " commandName " " usage);  \
        return;                                                                                       \
      }                                                                                               \
      functionCall(arg);                                                                              \
      return;                                                                                         \
    }
    APP_CONSOLE_AS_COMMAND("exec", Exec, "someobject.somemethod(42)")
    APP_CONSOLE_AS_COMMAND("eval", Eval, "2 + 2 * 2")
    APP_CONSOLE_AS_COMMAND("addvar", AddVariable, "int a = 5")
    APP_CONSOLE_AS_COMMAND("delvar", AddVariable, "a")
    APP_CONSOLE_AS_COMMAND("addfunc", AddVariable, "*WIP*")
    APP_CONSOLE_AS_COMMAND("delfunc", AddVariable, "*WIP*")
    else if (cmd == "listvars") {
      ListVariables();
      return;
    }
    else if (cmd == "listfuncs") {
      // TODO: list functions
      AddLog("WIP");
      return;
    }
    
    AddLog("Unkown command");
  }

  void AppConsole::Exec(std::string &arg) {
    asIScriptModule *mod = m_scriptEngine->GetModule("console", asGM_CREATE_IF_NOT_EXISTS);

    if (arg.length() > 0 && arg[arg.length()-1] != ';')
		  arg += ";";

    int r = ExecuteString(m_scriptEngine, arg.c_str(), mod);

    if (r < 0) {
      AddLog("[error] Invalid script statement.");
    }
    else if (r == asEXECUTION_EXCEPTION) {
      AddLog("[error] A script exception was raised.");
    }
    else {
      AddLog("Executed.");
    }
  }
  void AppConsole::Eval(std::string &arg) {
    asIScriptModule *mod = m_scriptEngine->GetModule("console", asGM_CREATE_IF_NOT_EXISTS);

    std::string script = "_grab(" + arg + ");";
    int r = ExecuteString(m_scriptEngine, script.c_str(), mod);

    if (r < 0) {
      AddLog("[error] Invalid script statement.");
    }
    else if (r == asEXECUTION_EXCEPTION) {
      AddLog("[error] A script exception was raised.");
    }
  }
  void AppConsole::AddVariable(std::string &arg) {
    asIScriptModule *mod = m_scriptEngine->GetModule("console", asGM_CREATE_IF_NOT_EXISTS);

    if (arg.length() > 0 && arg[arg.length()-1] != ';')
		  arg += ";";
    
    int r = mod->CompileGlobalVar("addvar", arg.c_str(), 0);
    if (r < 0) {
      // TODO: Add better description of error (invalid declaration, name conflict, etc)
      AddLog("[error] Failed to add variable.");
    }
    else
      AddLog("Variable added.");
  }
  void AppConsole::DeleteVariable(std::string &arg)
  {
    asIScriptModule *mod = m_scriptEngine->GetModule("console");
    if (mod == 0 || mod->GetGlobalVarCount() == 0) {
      AddLog("No variables have been added.");
      return;
    }

    // Trim the string to find the variable name
    size_t p1 = arg.find_first_not_of(" \n\r\t");
    if (p1 != std::string::npos)
      arg = arg.substr(p1, -1);
    size_t p2 = arg.find_last_not_of(" \n\r\t");
    if (p2 != std::string::npos)
      arg = arg.substr(0, p2+1);

    int index = mod->GetGlobalVarIndexByName(arg.c_str());
    if (index >= 0) {
      mod->RemoveGlobalVar(index);
      AddLog("Variable removed.");
    }
    else
      AddLog("[error] No such variable.");
  }
  void AppConsole::AddFunction(std::string &arg)
  {
    asIScriptModule *mod = m_scriptEngine->GetModule("console", asGM_CREATE_IF_NOT_EXISTS);

    asIScriptFunction *func = 0;
    int r = mod->CompileFunction("addfunc", arg.c_str(), 0, asCOMP_ADD_TO_MODULE, &func);
    if (r < 0) {
      // TODO: Add better description of error (invalid declaration, name conflict, etc)
      AddLog("[error] Failed to add function.");
    } 
    else {
      // The script engine supports function overloads, but to simplify the 
      // console we'll disallow multiple functions with the same name.
      // We know the function was added, so if GetFunctionByName() fails it is
      // because there already was another function with the same name.
      if( mod->GetFunctionByName(func->GetName()) == 0 ) {
        mod->RemoveFunction(func);
        AddLog("[error] Another function with that name already exists.");
      }
      else
        AddLog("Function added.");
    }

    // We must release the function object
    if (func)
      func->Release();
  }
  void AppConsole::DeleteFunction(std::string &arg)
  {
    asIScriptModule *mod = m_scriptEngine->GetModule("console");
    if( mod == 0 || mod->GetFunctionCount() == 0 ) 
    {
      AddLog("No functions have been added.");
      return;
    }

    // trim the string to find the variable name
    size_t p1 = arg.find_first_not_of(" \n\r\t");
    if( p1 != std::string::npos )
      arg = arg.substr(p1, -1);
    size_t p2 = arg.find_last_not_of(" \n\r\t");
    if( p2 != std::string::npos )
      arg = arg.substr(0, p2+1);

    asIScriptFunction *func = mod->GetFunctionByName(arg.c_str());
    if( func )
    {
      mod->RemoveFunction(func);
      AddLog("Function removed.");
    }
    else
      AddLog("[error] No such function.");

    // Since functions can be recursive, we'll call the garbage
    // collector to make sure the object is really freed
    m_scriptEngine->GarbageCollect();
  }
  void AppConsole::ListVariables()
  {
    asUINT n;

    // List the application registered variables
    AddLog("Application variables:");
    for (n = 0; n < (asUINT)m_scriptEngine->GetGlobalPropertyCount(); n++)
    {
      const char *name;
      int typeId;
      bool isConst;
      m_scriptEngine->GetGlobalPropertyByIndex(n, &name, 0, &typeId, &isConst);
      std::string decl = isConst ? " const " : " ";
      decl += m_scriptEngine->GetTypeDeclaration(typeId);
      decl += " ";
      decl += name;
      AddLog("%s", decl.c_str());
    }

    // List the user variables in the module
    asIScriptModule *mod = m_scriptEngine->GetModule("console");
    if (mod)
    {
      AddLog("");
      AddLog("User variables:");
      for( n = 0; n < (asUINT)mod->GetGlobalVarCount(); n++ )
      {
        AddLog("%s", mod->GetGlobalVarDeclaration(n));
      }
    }
  }

  void AppConsole::_Grab(int v) {
    AddLog("%i", v);
  }
  void AppConsole::_Grab(unsigned int v) {
    AddLog("%u", v);
  }
  void AppConsole::_Grab(bool v) {
    AddLog(v ? "true" : "false");
  }
  void AppConsole::_Grab(float v) {
    AddLog("%f", v);
  }
  void AppConsole::_Grab(double v) {
    AddLog("%f", v);
  }
  void AppConsole::_Grab(const std::string &v) {
    AddLog("\"%s\"", v.c_str());
  }
  void AppConsole::_Grab() {
    // no value
  }

  AppConsole *AppConsole::GetInstance() {
    return instance;
  }
}