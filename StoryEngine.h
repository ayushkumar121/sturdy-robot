// Created by ari on 2/25/26.

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class StoryEngine {
public:
    enum class SpeakerPosition {
        LEFT, RIGHT
    };

    struct Dialogue {
        std::string speaker;
        SpeakerPosition speakerPosition;
        std::string text;
        std::string characterSpritePath;
        std::string backgroundPath;
    };

    struct Choice {
        std::string label;
        std::string targetNodeId;
    };

    struct StoryNode {
        std::string id;
        std::vector<Dialogue> dialogues;
        std::vector<Choice> choices;
        std::string nextNodeId; // In case of no dialogues
        std::unordered_map<std::string, std::string> setFlags;
    };

    void loadStory();

    const Dialogue& getCurrentDialogue() const;
    const std::vector<Choice>& getChoices();
    bool isDialogueFinished() const;

    void advance();
    void choose(int choiceIndex);
private:
    std::string currentNodeId;
    size_t currentDialogueId = 0;
    std::unordered_map<std::string, StoryNode> nodes;
    std::unordered_map<std::string, std::string> flags;
};
