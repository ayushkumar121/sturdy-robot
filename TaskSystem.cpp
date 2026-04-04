// Created by ari on 2/28/26.

#include "TaskSystem.h"

#include <iostream>
#include <ostream>
#include <utility>

const TaskSystem::Message& TaskSystem::Task::getCurrentMessage() {
	return messages.at(currentMessage);
}

void TaskSystem::Task::chooseReply(std::string choice) {
	currentMessage = std::move(choice);
}

void TaskSystem::Task::reset() {
	currentMessage = startMessage;
}

TaskSystem& TaskSystem::getInstance() {
	static TaskSystem taskSystem;
	return taskSystem;
}

TaskSystem::TaskSystem() {
	// TODO: load from json
	auto task = Task{
		"task_01",
		{
			{
				"message_01",
				{
					"Hi Alex, How can i help you today?",
					{
						{"My name is @player", "message_02"},
						{"Make this poster more relatable", "message_03"},
						{"Add a new background", ""}
					},
				{}
				}
			},
			{
				"message_02",
				{
					"Sorry alex, didn't know you changed your name, are you running from the law?"
					"As per the company policy i would have to report you to the authorities."
					"Do you want me to go ahead and report you?",
					{
						{"Go right ahead", "message_05"},
						{"No, i'm not not a fugitive", "message_06"},
					},
				{}
				}
			},
			{
				"message_03",
				{
					"Sure here is your poster but more relatable",
					{
						{"Remove the text, add a friendly face instead", "message_07"},
						{"Good, Now add a background", ""},
					},
				{
					{
						"poster",
						{
							FileType::IMAGE,
							"relatable-poster.png",
							"assets/sprites/relatable-poster.jpg"
						}
					}
				}
				}
			},
			{
				"message_04",
				{
					"Sure, here is your with a popular background!",
					{
						{"No, change it to a wallpaper of ocean", ""},
					},
				{}
				}
			},
			{
				"message_05",
				{
					"Communication services are currently disabled request cannot be completed",
					{},
					{}
				}
			},
			{
				"message_06",
				{
					"Glad to hear it, most young people these days have a tendency to defy"
					"authorities. I'm happy you are not one of them otherwise i would have "
					"reported you",
					{},
					{}
				}
			},
			{
				"message_07",
				{
					"Sure, here is a friendly face.",
					{},
					{}
				}
			}
		},
		"message_01",
		"message_01",
	};
	tasks.insert({task.id, task});
}

TaskSystem::Task& TaskSystem::getTask(const std::string& id) {
	return tasks.at(id);
}