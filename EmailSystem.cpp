// Created by ari on 3/5/26.

#include "EmailSystem.h"

EmailSystem& EmailSystem::getInstance() {
	static EmailSystem instance;
	return instance;
}

EmailSystem::EmailSystem() {
	// TODO: read from file
	emails.emplace_back(
		"trent@future.inc", "Getting Started",
		"How to get started with future industries os",
		EmailState::NEW,
		std::nullopt
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
	emails.emplace_back(
		"trent@future.inc", "Your First Task",
		"Our previous intern left without finishing his task, pleas complete his task",
		EmailState::NEW,
		"task_01"
	);
}

const std::vector<EmailSystem::Email>& EmailSystem::getEmails() const {
	return emails;
}