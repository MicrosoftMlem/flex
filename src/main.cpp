#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <set>
#include <print>

// if no args provided, run full flex in the current directory

// comment = 20
// normal line = 10
// include = 20
// funtion call = 30
// function declaration = 40
// type = 15
// file = 400
// .gitignore = 200
// .md = 200
// if = 25
// while = 30
// for = 30

std::unordered_map<std::string, int> scores {
  {"comment", 20}, {"line", 10}, {"include", 20}, {"call", 30},
      {"declaration", 40}, {"type", 15}, {"file", 400}, {"gitignore", 200},
      {"markdown", 200}, {"if", 25}, {"while", 30},
      {"for", 30}
};

void evaluateCodeLineScore(int &totScore, const std::string &line) {
  if (line != "") {
    totScore += scores["line"];
  }
  if (line.find("//") != std::string::npos) {
    totScore += scores["comment"];
  }
  if (line.find("#include") != std::string::npos) {
    totScore += scores["include"];
  }
}

void evaluateOtherLineScore(int &totScore, const std::string &line) {
  if (line != "") {
    totScore += scores["line"]; 
  }
}

// currently only c/cpp/h bc thats all i scan for rn
static const std::set<std::string> exts = {
    ".cpp",
    ".h",
    ".hpp",
    ".c",
    ".cc",
    "cxx"
};

const std::set<std::string> disallowedDirs = {
    "build",
    "cache",
    ".cache",
    ".git",
    "node_modules",
    "out"
};

int main(int argc, char *argv[]) {
  int totalScore = 0;
  auto cwd = std::filesystem::current_path();
  
  if (argc > 1) {
    std::filesystem::path path = argv[1];
    if (!std::filesystem::exists(path)) {
      std::println("Couldn't find path: {}", argv[1]);
      return -1;
    } else if (std::filesystem::is_directory(path)) {
      cwd = path;
    } else if (std::filesystem::is_regular_file(path)) {
      cwd = path;
      if (exts.count(cwd.extension().string())) {
        std::fstream f(cwd);
        std::string line;
        while (std::getline(f, line)) {
          evaluateCodeLineScore(totalScore, line);
        }
      } else if (cwd.extension() == ".md" || cwd.extension() == ".gitignore" ||
                 cwd.extension() == "txt") {
        totalScore += scores["markdown"];
        std::fstream f(cwd);
        std::string line;
        while (std::getline(f, line)) {
          evaluateCodeLineScore(totalScore, line);
        }
      }

      std::println("File has a total score of... {}!", totalScore);
      return 0;
    }
  } else {
    std::println("No path specified, using current working directory.");
  }


  for (auto it = std::filesystem::recursive_directory_iterator(cwd);
       it != std::filesystem::recursive_directory_iterator(); ++it) {
    const auto &entry = *it;

    if (entry.is_directory() && disallowedDirs.count(entry.path().filename())) {
      it.disable_recursion_pending();
      continue;
    }
    if (entry.is_regular_file()) {
      if (exts.count(entry.path().extension().string())) {
	std::fstream f(entry.path());
	std::string line;
        while (std::getline(f, line)) {
	  evaluateCodeLineScore(totalScore, line);
	}
      } else if (entry.path().extension() == ".md" ||
                 entry.path().extension() == ".gitignore" ||
                 entry.path().extension() == ".txt") {
        totalScore += scores["markdown"];
        std::fstream f(entry.path());
        std::string line;
        while (std::getline(f, line)) {
          evaluateOtherLineScore(totalScore, line);          
        }
      }
    }
  }

  std::println("Project has a total score of... {}!", totalScore);

  return 0;
}
