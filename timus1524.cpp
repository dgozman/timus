#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
bool __hack1 = std::ios::sync_with_stdio(false);
auto __hack2 = cin.tie(nullptr);
template<typename T> using vec = vector<T>;
template<typename T> using vec2d = vec<vec<T>>;
template<typename T> using vec3d = vec<vec2d<T>>;
using i64 = int64_t;
using u64 = uint64_t;
using i32 = int32_t;
using u32 = uint32_t;
using i8 = int8_t;
using u8 = uint8_t;

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  i64 ni64() const { return next<i64>(); }
  string ns() const { return next<string>(); }
  template<class T> vec<T> nvec(int n) const { vec<T> v(n); for (int i = 0; i < n; ++i) v[i] = next<T>(); return v; }
  vec<int> nvi(int n) const { return nvec<int>(n); }
  string getline() { string s; std::getline(*in, s); return s; }
};

struct Output {
  Output(ostream &out) : out(&out) {}
  ostream *out;
  inline void print() {}
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
};

template<typename T> vec2d<T> newVec2d(int n, int m, const T& init) {
  vec2d<T> v(n, vec<T>(m, init));
  return v;
}

template<typename T> vec3d<T> newVec3d(int n, int m, int k, const T& init) {
  vec3d<T> v(n, newVec2d(m, k, init));
  return v;
}

void panic() {
  int z = 0;
  z = 1 / z;
}

template<typename T> bool rmn(T& value, const T& candidate) {
  if (candidate < value) { value = candidate; return true; }
  return false;
}

template<typename T> bool rmx(T& value, const T& candidate) {
  if (candidate > value) { value = candidate; return true; }
  return false;
}

void solve(Input& in, Output& out);
int main() {
  // ifstream fin("input.txt");
  // Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
  return 0;
}

// Whether delivery quests are not considered as two separate rounding segments, but as a single one.
// true == fails test #1.
const bool kFeatureDeliverySingleSegment = false;
// Whether the minimum negotiation experience applies to each agent separately.
// true == fails test #4.
const bool kFeatureHardMinExperience = false;
// Whether to round parameters to two digits after decimal.
// true == fails test #2.
const bool kFeatureRoundTo100 = false;
// Whether the car does not "break" when its resource is used exactly.
// true == fails test #3.
const bool kFeatureNoIdleBreaks = false;

// Whether the maximum walking distance is a hard limit on walking.
// Low impact.
const bool kFeatureMaxWalkingDistanceLimit = false;
// Whether to uppercase the first letter of the agent name when choosing the letter.
const bool kUppercaseFirstLetter = true;
// Whether to compare stats using epsilon.
const bool kFeatureEps = false;

struct AgentParams {
  double accuracy, walking_speed, intelligence, experience, driving_skill;
};

struct Agent {
  string name;
  AgentParams params;
  string letter;
  bool busy;
};

struct CarType {
  string name;
  double min_driving_skill;
  double max_distance;
};

struct Car {
  double min_driving_skill;
  double remaining_distance;
  string id;
  bool busy;
};

struct MonsterKind {
  double min_experience, min_intelligence;
  double evasiveness, experience_gain;
  string name;
};

const int kQuestDelivery = 0;
const int kQuestMonsterKill = 1;
const int kQuestInvestigation = 2;
const int kQuestNegotiation = 3;
struct Quest {
  int index;
  int type;
  double distance;
  string monster;
  double min_intelligence, min_investigation_time;
  double min_experience, min_negotiation_time;
};

struct QuestAction {
  Quest quest;
  int end_time;
  bool two_agents;
  string agent1, agent2;
  bool using_car;
  string car_id;

  int car_break_time;  // -1 if does not break
  int action_end_time;  // interesting for the monster kill
  double walked, driven;

  QuestAction(const Quest& quest): quest(quest) {}
};

// Must be sorted in the right order.
const int kResultCarBought = 0;
const int kResultCarBroken = 1;
const int kResultMonsterKilled = 2;
const int kResultQuestFinished = 3;
const int kResultAgentRetired = 4;
const int kResultAgentArrived = 5;
const int kResultQuestStarted = 6;
// time, type, description
using ResultLine = tuple<int, int, string>;

int round2(double x) {
  return round(x + 1e-8);
}

double round100(double x) {
  return kFeatureRoundTo100 ? round2(x * 100) / 100 : x;
}

bool is_greater_or_equal(double value, double then) {
  return kFeatureEps ? value > then - 1e-9 : value >= then;
}

void solve(Input& in, Output& out) {
  map<string, Agent> agents;
  map<string, CarType> car_types;
  map<string, Car> cars;
  map<string, MonsterKind> monster_kinds;
  double max_walking_distance, max_evasiveness, max_experience_for_killing, retirement_experience;
  vec<Quest> pending_quests;
  multimap<int, QuestAction> ending_quests;
  vec<ResultLine> result;

  auto car_broken = [&] (int time, const string& id, bool is_idle_break) {
    if (!kFeatureNoIdleBreaks || !is_idle_break)
      result.push_back(make_tuple(time, kResultCarBroken, "Car " + id + " was broken."));
    cars.erase(id);
  };

  auto choose_agent_letter = [&] (const string& name) {
    char first = name[0];
    if (kUppercaseFirstLetter && first >= 'a' && first <= 'z')
      first = first + 'A' - 'a';
    char best = '\0';
    for (char letter = 'A'; letter <= 'Z'; letter++) {
      if (agents.count(string(1, letter)))
        continue;
      int dist = abs(letter - first);
      int best_dist = abs(best - first);
      if (best == '\0' || dist < best_dist)
        best = letter;
    }
    if (best == '\0')
      panic();
    return string(1, best);
  };

  auto combine_params = [&] (const AgentParams& p1, const AgentParams& p2) {
    AgentParams combined;
    combined.walking_speed = min(p1.walking_speed, p2.walking_speed);
    combined.driving_skill = max(p1.driving_skill, p2.driving_skill);
    combined.accuracy = (p1.accuracy + p2.accuracy) / 2;
    combined.experience = 1 - (1 - p1.experience) * (1 - p2.experience);
    combined.intelligence = 1 - (1 - p1.intelligence) * (1 - p2.intelligence);
    return combined;
  };

  auto choose_car = [&] (double distance, const AgentParams& params, QuestAction& action, int action_time) {
    // fills using_car
    // fills car_id
    // fills car_break_time
    // fills action_end_time
    // fills walked, driven
    // updates end_time
    int start_time = action.end_time;
    bool can = false;

    int walk_time = round2(distance / params.walking_speed);
    action.using_car = false;
    action.walked = 2 * distance;
    action.driven = 0;
    action.action_end_time = start_time + walk_time + action_time;
    action.end_time = start_time + 2 * walk_time + action_time;
    if (kFeatureDeliverySingleSegment && !action_time) {
      walk_time = round2(2 * distance / params.walking_speed);
      action.end_time = start_time + walk_time + action_time;
    }
    if (!kFeatureMaxWalkingDistanceLimit || is_greater_or_equal(max_walking_distance, action.walked))
      can = true;

    for (auto& id_car : cars) {
      const Car& car = id_car.second;
      if (car.busy || !is_greater_or_equal(params.driving_skill, car.min_driving_skill))
        continue;
      double drive1_distance = min(distance, car.remaining_distance);
      double walk1_distance = distance - drive1_distance;
      double drive2_distance = min(distance, car.remaining_distance - drive1_distance);
      double walk2_distance = distance - drive2_distance;
      if (kFeatureDeliverySingleSegment && !action_time) {
        drive1_distance = min(2 * distance, car.remaining_distance);
        walk1_distance = 2 * distance - drive1_distance;
        drive2_distance = 0;
        walk2_distance = 0;
      }
      if (kFeatureMaxWalkingDistanceLimit && !is_greater_or_equal(max_walking_distance, walk1_distance + walk2_distance))
        continue;
      int drive1_time = round2(drive1_distance / params.driving_skill);
      int walk1_time = round2(walk1_distance / params.walking_speed);
      int drive2_time = round2(drive2_distance / params.driving_skill);
      int walk2_time = round2(walk2_distance / params.walking_speed);
      if (!can || (drive1_time + walk1_time + drive2_time + walk2_time + action_time < action.end_time - start_time)) {
        can = true;
        action.using_car = true;
        action.car_id = car.id;
        action.action_end_time = start_time + drive1_time + walk1_time + action_time;
        action.end_time = start_time + drive1_time + walk1_time + drive2_time + walk2_time + action_time;
        action.walked = walk1_distance + walk2_distance;
        action.driven = drive1_distance + drive2_distance;
        if (drive1_distance >= car.remaining_distance) {
          action.car_break_time = start_time + drive1_time;
        } else if (drive1_distance + drive2_distance >= car.remaining_distance) {
          action.car_break_time = start_time + drive1_time + walk1_time + action_time + drive2_time;
        } else {
          action.car_break_time = -1;
        }
      }
    }
    return can;
  };

  auto can_perform_quest = [&] (int start_time, const Quest& quest, const AgentParams& params, QuestAction& action) {
    action.end_time = start_time;
    if (quest.type == kQuestDelivery) {
      return choose_car(quest.distance, params, action, 0);
    } else if (quest.type == kQuestMonsterKill) {
      MonsterKind& monster_kind = monster_kinds[quest.monster];
      if (!is_greater_or_equal(params.experience, monster_kind.min_experience) ||
          !is_greater_or_equal(params.intelligence, monster_kind.min_intelligence)) {
        return false;
      }
      int kill_time = round2(monster_kind.evasiveness / params.accuracy);
      return choose_car(quest.distance, params, action, kill_time);
    } else if (quest.type == kQuestInvestigation) {
      if (!is_greater_or_equal(params.intelligence, quest.min_intelligence))
        return false;
      int investigation_time = round2(quest.min_investigation_time / params.intelligence);
      return choose_car(quest.distance, params, action, investigation_time);
    } else if (quest.type == kQuestNegotiation) {
      if (!is_greater_or_equal(params.experience, quest.min_experience))
        return false;
      int negotiation_time = round2(quest.min_negotiation_time / params.experience);
      return choose_car(quest.distance, params, action, negotiation_time);
    } else {
      panic();
      return false;
    }
  };

  auto can_start_quest = [&] (int start_time, const Quest& quest, QuestAction& action) {
    bool can = false;
    for (auto& id_agent1 : agents) {
      const Agent& agent1 = id_agent1.second;
      if (agent1.busy)
        continue;
      AgentParams one_agent_params = agent1.params;
      QuestAction one_agent_action(quest);
      if (can_perform_quest(start_time, quest, one_agent_params, one_agent_action)) {
        if (!can || one_agent_action.end_time < action.end_time) {
          can = true;
          action = one_agent_action;
          action.two_agents = false;
          action.agent1 = agent1.letter;
        }
      }
    }
    for (auto& id_agent1 : agents) {
      const Agent& agent1 = id_agent1.second;
      if (agent1.busy)
        continue;
      for (auto& id_agent2 : agents) {
        const Agent& agent2 = id_agent2.second;
        if (agent2.busy || agent2.letter <= agent1.letter)
          continue;
        if (kFeatureHardMinExperience && quest.type == kQuestNegotiation &&
            (!is_greater_or_equal(agent1.params.experience, quest.min_experience) ||
             !is_greater_or_equal(agent1.params.experience, quest.min_experience))) {
          continue;
        }
        AgentParams two_agent_params = combine_params(agent1.params, agent2.params);
        QuestAction two_agent_action(quest);
        if (can_perform_quest(start_time, quest, two_agent_params, two_agent_action)) {
          if (!can || two_agent_action.end_time < action.end_time) {
            can = true;
            action = two_agent_action;
            action.two_agents = true;
            action.agent1 = agent1.letter;
            action.agent2 = agent2.letter;
          }
        }
      }
    }
    return can;
  };

  auto action_agents_to_string = [&] (const QuestAction& action) {
    return action.agent1 + (action.two_agents ? " and agent " + action.agent2 : "");
  };

  auto maybe_start_quests = [&] (int current_time) {
    bool started;
    do {
      started = false;
      for (int i = 0; i < pending_quests.size(); i++) {
        QuestAction action(pending_quests[i]);
        if (can_start_quest(current_time, pending_quests[i], action)) {
          result.push_back(make_tuple(current_time, kResultQuestStarted,
            "Agent " + action_agents_to_string(action) +
            " started quest " + to_string(action.quest.index) +
            (action.using_car ? " using car " + action.car_id : "") +
            "."));
            //  + "(end=" + to_string(action.end_time) + ")"));
          agents[action.agent1].busy = true;
          if (action.two_agents)
            agents[action.agent2].busy = true;
          if (action.using_car)
            cars[action.car_id].busy = true;
          ending_quests.emplace(action.end_time, action);
          pending_quests.erase(pending_quests.begin() + i);
          started = true;
          break;
        }
      }
    } while (started);
  };

  auto update_stats_after_quest = [&] (AgentParams& params, const QuestAction& action) {
    params.walking_speed = round100(params.walking_speed + (1 - params.walking_speed) * action.walked / max_walking_distance);
    params.driving_skill = round100(params.driving_skill + (1 - params.driving_skill) * action.driven / max_walking_distance);
    if (action.quest.type == kQuestMonsterKill) {
      params.experience = round100(params.experience + (1 - params.experience) * monster_kinds[action.quest.monster].experience_gain / max_experience_for_killing);
      params.accuracy = round100(params.accuracy + (1 - params.accuracy) * monster_kinds[action.quest.monster].evasiveness / max_evasiveness);
    } else if (action.quest.type == kQuestInvestigation) {
      params.experience = round100(params.experience + (1 - params.experience) * params.intelligence / action.quest.min_investigation_time);
      params.intelligence = round100(params.intelligence + (1 - params.intelligence) * params.intelligence / action.quest.min_investigation_time);
    } else if (action.quest.type == kQuestNegotiation) {
      params.experience = round100(params.experience + (1 - params.experience) * params.experience / action.quest.min_negotiation_time);
    }
  };

  auto maybe_retire = [&] (int retire_time, string letter) {
    if (is_greater_or_equal(agents[letter].params.experience, retirement_experience)) {
      result.push_back(make_tuple(retire_time, kResultAgentRetired, "Agent " + letter + " has tired."));
      agents.erase(letter);
    }
  };

  auto quest_ended = [&] (const QuestAction& action) {
    if (action.using_car) {
      cars[action.car_id].busy = false;
      cars[action.car_id].remaining_distance -= action.driven;
      if (action.car_break_time != -1)
        car_broken(action.car_break_time, action.car_id, action.car_break_time == action.end_time);
    }
    if (action.quest.type == kQuestMonsterKill) {
      result.push_back(make_tuple(action.action_end_time, kResultMonsterKilled,
        "Agent " + action_agents_to_string(action) +
        " killed monster " + action.quest.monster + "."));
    }
    string description = "Agent " + action_agents_to_string(action) +
      " finished quest " + to_string(action.quest.index) + ".";
    Agent& agent1 = agents[action.agent1];
    update_stats_after_quest(agent1.params, action);
    // description += " (" + action.agent1 + " e=" + to_string(agent1.params.experience) + ")";
    agent1.busy = false;
    maybe_retire(action.end_time, agent1.letter);
    if (action.two_agents) {
      Agent& agent2 = agents[action.agent2];
      update_stats_after_quest(agent2.params, action);
      // description += " (" + action.agent2 + " e=" + to_string(agent2.params.experience) + ")";
      agent2.busy = false;
      maybe_retire(action.end_time, agent2.letter);
    }
    result.push_back(make_tuple(action.end_time, kResultQuestFinished, description));
  };

  auto pump_quests = [&] (int before_time) {
    while (ending_quests.size() > 0) {
      auto it = ending_quests.begin();
      int end_time = it->first;
      if (end_time > before_time)
        break;

      while (ending_quests.size() > 0) {
        it = ending_quests.begin();
        if (it->first != end_time)
          break;

        QuestAction action = it->second;
        ending_quests.erase(it);
        quest_ended(action);
      }

      if (end_time < before_time)
        maybe_start_quests(end_time);
    }
  };

  int tmp = in.ni();
  for (int i = 0; i < tmp; i++) {
    Agent agent;
    agent.name = in.ns();
    agent.params.accuracy = in.next<double>();
    agent.params.walking_speed = in.next<double>();
    agent.params.intelligence = in.next<double>();
    agent.params.experience = in.next<double>();
    agent.params.driving_skill = in.next<double>();
    agent.letter = in.ns();
    agent.busy = false;
    agents[agent.letter] = agent;
  }

  tmp = in.ni();
  for (int i = 0; i < tmp; i++) {
    CarType car_type;
    car_type.min_driving_skill = in.next<double>();
    car_type.max_distance = in.next<double>();
    car_type.name = in.ns();
    car_types[car_type.name] = car_type;
  }

  tmp = in.ni();
  for (int i = 0; i < tmp; i++) {
    Car car;
    string type = in.ns();
    car.min_driving_skill = car_types[type].min_driving_skill;
    car.remaining_distance = car_types[type].max_distance - in.next<double>();
    car.id = in.ns();
    car.busy = false;
    cars[car.id] = car;
    if (car.remaining_distance < 1e-3)
      car_broken(0, car.id, true);
  }

  tmp = in.ni();
  for (int i = 0; i < tmp; i++) {
    MonsterKind monster_kind;
    monster_kind.min_experience = in.next<double>();
    monster_kind.min_intelligence = in.next<double>();
    monster_kind.evasiveness = in.next<double>();
    monster_kind.experience_gain = in.next<double>();
    monster_kind.name = in.ns();
    monster_kinds[monster_kind.name] = monster_kind;
  }

  max_walking_distance = in.next<double>();
  max_evasiveness = in.next<double>();
  max_experience_for_killing = in.next<double>();
  retirement_experience = in.next<double>();

  int quest_index = 0;
  for (tmp = in.ni(); tmp > 0; tmp--) {
    int event_time = in.ni();
    pump_quests(event_time);

    string event_type = in.ns();
    if (event_type == "newagent") {
      Agent agent;
      agent.name = in.ns();
      agent.params.accuracy = in.next<double>();
      agent.params.walking_speed = in.next<double>();
      agent.params.intelligence = in.next<double>();
      agent.params.experience = in.next<double>();
      agent.params.driving_skill = in.next<double>();
      agent.letter = choose_agent_letter(agent.name);
      agent.busy = false;
      agents[agent.letter] = agent;
      result.push_back(make_tuple(event_time, kResultAgentArrived, "New agent " + agent.name + " got a letter " + agent.letter + "."));
      maybe_start_quests(event_time);
      continue;
    }

    if (event_type == "newcar") {
      Car car;
      string type = in.ns();
      car.min_driving_skill = car_types[type].min_driving_skill;
      car.remaining_distance = car_types[type].max_distance - in.next<double>();
      car.id = in.ns();
      car.busy = false;
      cars[car.id] = car;
      result.push_back(make_tuple(event_time, kResultCarBought, "MIB bought a car of class " + type + "."));
      maybe_start_quests(event_time);
      continue;
    }

    if (event_type != "quest")
      panic();
    string quest_type = in.ns();
    Quest quest;
    quest.index = ++quest_index;
    if (quest_type == "run") {
      quest.type = kQuestDelivery;
      quest.distance = in.next<double>();
    } else if (quest_type == "kill") {
      quest.type = kQuestMonsterKill;
      quest.distance = in.next<double>();
      quest.monster = in.ns();
    } else if (quest_type == "findout") {
      quest.type = kQuestInvestigation;
      quest.distance = in.next<double>();
      quest.min_intelligence = in.next<double>();
      quest.min_investigation_time = in.next<double>();
    } else if (quest_type == "talk") {
      quest.type = kQuestNegotiation;
      quest.distance = in.next<double>();
      quest.min_experience = in.next<double>();
      quest.min_negotiation_time = in.next<double>();
    } else {
      panic();
    }
    pending_quests.push_back(quest);
    maybe_start_quests(event_time); // can optimize by passing the quest
  }

  pump_quests(2000000000);
  if (pending_quests.size())
    panic();

  sort(result.begin(), result.end());
  for (auto& r : result) {
    int time = get<0>(r);
    int minutes = time % 60;
    int hours = (time / 60) % 24;
    int days = time / (24 * 60);
    ostringstream stream;
    stream << std::setw(4) << std::setfill('0') << days << ":" << std::setw(2) << hours << ":" << std::setw(2) << minutes << "    " << get<2>(r);
    out(stream.str());
  }
}
