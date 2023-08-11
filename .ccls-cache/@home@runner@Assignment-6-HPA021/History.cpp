

#include <cassert>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

#include "History.h"

History::History() : head(nullptr) {}
 
History::History(const History &history_in){
   head = copyLinkedList(history_in.head);
 }
History::~History(){
  destroyLinkedList(head);
}
History &History::operator=(const History &history_in){
  if(this  == &history_in){
    return *this;
  }
  
  destroyLinkedList(head);
  
  head = copyLinkedList(history_in.head);
  
  return *this;
}

bool History::isAtLeast1Long() const { return head != nullptr; }

void History::addAtHead(const Board &board_in) {
  HistoryNode *new_Node = new HistoryNode;
  new_Node->board = board_in;
  new_Node->p_next = head;
  head = new_Node;
}

const Board &History::getFirst() const {
  assert(head != nullptr);
  return head->board;
}

bool History::isAtLeast2Long() const {
  if (head != nullptr && head->p_next != nullptr) {
    return true;
  }
  return false;
}

bool History::isAtLeast3Long() const {
  if (head != nullptr && head->p_next != nullptr &&
      head->p_next->p_next != nullptr) {
    return true;
  }
  return false;
}

const Board &History::getSecond() const { return head->p_next->board; }
bool History::isInList(const Board &board_in) const {
  HistoryNode *play = head;
  while (play != nullptr) {
    if (play->board == board_in) {
      return true;
    }
    play = play->p_next;
  }

  return false;
}

void History::removeAtHead() {
  if (head != nullptr) {
    HistoryNode *temp = head;
    head = head->p_next;
    delete temp;
  }
}

void History::removeAll(){
  destroyLinkedList(head);
  head = nullptr;
}

HistoryNode* copyLinkedList (const HistoryNode*p_old_head_in){
  if(p_old_head_in == nullptr){
    return nullptr;
  }
//creating the new node for the head and the copy it 
  HistoryNode* p_new_head = new HistoryNode;
   p_new_head->board = p_old_head_in->board; 

  HistoryNode* p_old_node = p_old_head_in->p_next;
  HistoryNode* p_new_node = p_new_head;

  while(p_old_node != nullptr){
    HistoryNode* p_new_temp =  new HistoryNode;
    p_new_temp->board = p_old_node->board;

   p_new_node->p_next = p_new_temp;
    

    p_old_node = p_old_node->p_next;
    p_new_node = p_new_node->p_next;
    
  }
  p_new_node->p_next = nullptr;
  
  return p_new_head;
}

void destroyLinkedList (HistoryNode* p_head_in){

  while(p_head_in != nullptr){
    HistoryNode* temp = p_head_in;
    p_head_in = p_head_in->p_next;
    delete temp;
  }
}

