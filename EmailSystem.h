// Created by ari on 3/5/26.

#pragma once

#include <string>
#include <vector>
#include <optional>

class EmailSystem {
public:
	enum class EmailState {
		NEW, READ, HIDDEN
	};

	struct Email {
		std::string from;
		std::string subject;
		std::string content;
		EmailState state;
		std::optional<std::string> taskId;
	};

	static EmailSystem& getInstance();
	const std::vector<Email>& getEmails() const;
private:
	EmailSystem();
	std::vector<Email> emails;
};