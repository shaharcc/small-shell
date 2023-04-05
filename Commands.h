#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>
#include <ctime>
#include <unistd.h>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define FAIL -1
#define EMPTY -1

typedef int jid_t;
using namespace std;

class Command {
  protected:
    const char* cmd_line;
    string cmd_no_ampersand;
    char *args[COMMAND_MAX_ARGS];
    int num_of_args;
  public:
    explicit Command(const char* cmd_line);
    virtual ~Command() = default;
    virtual void execute() = 0;
    const char* getCmdLine() {
      return cmd_line;
    }
    void setCmd(const char* cmd) {
        cmd_line = cmd;
    }
};

class BuiltInCommand : public Command {
 public:
  explicit BuiltInCommand(const char* cmd_line);
  virtual ~BuiltInCommand() {
      {
          delete cmd_line;
      }
  }
};

class TimeoutCommand : public BuiltInCommand {
    int time_out;
    string cmd;
public:
    explicit TimeoutCommand(const char* cmd_line, int timeout);
    virtual ~TimeoutCommand() = default;
    void execute() override;
    void addAlarm(pid_t pid) const;
};

class ExternalCommand : public Command {
private:
    bool is_background;
  public:
    bool is_alarm;
    ExternalCommand(const char* cmd_line, bool is_alarm, bool is_background);
    virtual ~ExternalCommand(){}
    void execute() override;
    bool isCmdComplex(string cmd);
    void timeoutExecute(TimeoutCommand* cmd);
};

class PipeCommand : public Command {
private:
    bool is_stdout;
 public:
  PipeCommand(const char* cmd_line, bool is_stdout);
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command {
private:
    string command;
    string file_name;
    bool is_append;
public:
    explicit RedirectionCommand(const char* cmd_line, bool append);
    virtual ~RedirectionCommand() = default;
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
  public:
    //char** p_previous_dir;
//    ChangeDirCommand(const char* cmd_line, char** p_previous_dir);
    ChangeDirCommand(const char* cmd_line);
    virtual ~ChangeDirCommand() = default;
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
  public:
    explicit GetCurrDirCommand(const char* cmd_line);
    virtual ~GetCurrDirCommand() = default;
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
  explicit ShowPidCommand(const char* cmd_line);
  virtual ~ShowPidCommand() = default;
  void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
public:
  explicit QuitCommand(const char* cmd_line);
  virtual ~QuitCommand() = default;
  void execute() override;
};

class JobsList {
 public:
  class JobEntry {
   public:
   jid_t jobId;
   pid_t jobPid;
   time_t creation_time;
   string command;
   bool is_stopped;
   int duration;
   JobEntry(jid_t jobId, pid_t jobPid, time_t creation_time, string& command, bool is_stopped, int duration);
   time_t getElapsedTime() const;
  };
  
 public:
    vector<JobEntry> jobs_list;
    jid_t jobs_num;

    JobsList();
    ~JobsList() = default;
    void addJob(string cmd, pid_t pid, int duration = 0, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry* getJobById(jid_t jobId);
    JobEntry* getJobByPId(pid_t jobId);
    void removeJobById(int jobId);
    JobEntry* getLastJob(int* lastJobId);
    int getMaxJidInList();
    JobEntry* getLastStoppedJob(int *jobId);
};

class ChpromptCommand : public BuiltInCommand {
public:
    explicit ChpromptCommand(const char* cmd_line);
    virtual ~ChpromptCommand() {}
    void execute() override;
};

class JobsCommand : public BuiltInCommand {
 public:
  JobsCommand(const char* cmd_line);
  virtual ~JobsCommand() {}
  void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
 public:
  explicit ForegroundCommand(const char* cmd_line);
  virtual ~ForegroundCommand();
  void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
 public:
  BackgroundCommand(const char* cmd_line);
  virtual ~BackgroundCommand() {}
  void execute() override;
};

class KillCommand : public BuiltInCommand {
 public:
  KillCommand(const char* cmd_line);
  virtual ~KillCommand() {}
  void execute() override;
};

class SmallShell {
    public:
        JobsList jobs_list;
        JobsList alarms_list;
        pid_t pid = getpid();
        string prompt = "smash";
        string previous_dir = "";

        jid_t fg_jid = EMPTY;
        pid_t curr_fg_pid = EMPTY;
        string current_cmd;
        string current_alarm_cmd;
        bool is_cmd_fg;
        bool is_fg_alarm;
        time_t current_duration;

        SmallShell();
        string getPrompt() {
            return prompt;
        }
  
        Command *CreateCommand(const char* cmd_line);
        SmallShell(SmallShell const&) = delete; // disable copy ctor
        void operator=(SmallShell const&)  = delete; // disable = operator
        static SmallShell& getInstance() {
        // make SmallShell singleton
            static SmallShell instance;
            // Instantiated on first use and guaranteed to be destroyed
            return instance;
        }
        ~SmallShell();
        void executeCommand(const char* cmd_line);
        JobsList::JobEntry* getTimedOutJob();
        time_t getMostRecentAlarmTime();
        void addTimeoutToAlarm(const char* cmd, pid_t pid, int duration);
};

#endif //SMASH_COMMAND_H_
