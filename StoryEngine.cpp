// Created by ari on 2/25/26.

#include "StoryEngine.h"

void StoryEngine::loadStory() {
    // TODO: load json
    auto node = StoryNode{
        "intro_01",
        {
            {
                "player",
                SpeakerPosition::LEFT,
                "You are nervously waiting in an office lobby looking around, everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy.everyone seems busy. They are all glued to their screen typing furiously as if they are angrily writing to someone, in a way they were.  He had perfect well combed hair and a black suit and underneath he was wearing a well ironed white shirt. He had perfect well combed hair and a black suit and underneath he was wearing a well ironed white shirt. He had perfect well combed hair and a black suit and underneath he was wearing a well ironed white shirt. He had perfect well combed hair and a black suit and underneath he was wearing a well ironed white shirt.",
                "assets/characters/VN_Visiter_A--Broad_Smile.png",
                "assets/backgrounds/officedark.jpg"
            },
            {
                "player",
                SpeakerPosition::LEFT,
                "Finally, after much waiting for your boss @boss comes around, he looks like one of those overconfident people in high school who have never faced any consequences in their life. He had perfect well combed hair and a black suit and underneath he was wearing a well ironed white shirt.",
                "assets/characters/VN_Visiter_A--Broad_Smile.png",
                "assets/backgrounds/officedark.jpg"
            },
            {
                "boss",
                SpeakerPosition::RIGHT,
                "Hi @player, how are you doing?",
                "assets/characters/VN_Visiter_C.png",
                "assets/backgrounds/officedark.jpg"
            }
        },
        {
            {
                "Never been better",
                "intro_02"
            },
            {
                "I’m quite nervous actually",
                "intro_03"
            }
        },
        "",
        {}
    };

    nodes.insert({node.id, node});
    currentNodeId = node.id;
    currentDialogueId = 0;
}

const StoryEngine::Dialogue& StoryEngine::getCurrentDialogue() const {
    const StoryNode& currentNode = nodes.at(currentNodeId);
    return currentNode.dialogues[currentDialogueId];
}

const std::vector<StoryEngine::Choice> & StoryEngine::getChoices() {
    return nodes.at(currentNodeId).choices;
}

bool StoryEngine::isDialogueFinished() const {
    const StoryNode& currentNode = nodes.at(currentNodeId);
    return currentDialogueId >= currentNode.dialogues.size();
}

void StoryEngine::advance() {
    const StoryNode& currentNode = nodes.at(currentNodeId);
    if (currentDialogueId < currentNode.dialogues.size()) {
        currentDialogueId = currentDialogueId+1;
    } else {
        currentDialogueId = 0;
        currentNodeId = nodes[currentNodeId].nextNodeId;
        currentDialogueId = 0;
    }
}

void StoryEngine::choose(int choiceIndex) {
    currentNodeId = nodes[currentNodeId].choices[choiceIndex].targetNodeId;
    currentDialogueId = 0;
}
