#include "Channel.h"
ClassImp(Channel);
Channel::Channel(){}
Channel::~Channel(){}
Channel::Channel(const Channel& ch){
    *this = ch;
}
Channel::Channel(Channel* ch){
    *this = *ch;
}