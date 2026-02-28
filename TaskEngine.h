// Created by ari on 2/28/26.

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class TaskEngine {
public:
	struct Choice {
		std::string text;
		std::string nextMessageId;
	};

	enum class FileType {IMAGE, DOCUMENT};

	struct File {
		FileType type;
		std::string name;
		std::string path;
	};

	struct Message {
		std::string text;
		std::vector<Choice> choices;
		std::unordered_map<std::string, File> files;
	};

	struct Task {
		std::string id;
		std::unordered_map<std::string, Message> messages;
		std::unordered_map<std::string, File> files;
		std::string currentMessage;

		const Message& getCurrentMessage();
		void chooseReply(std::string choice);
	};

	TaskEngine();
	Task& getTask(const std::string &id);
private:
	std::unordered_map<std::string, Task> tasks;
};