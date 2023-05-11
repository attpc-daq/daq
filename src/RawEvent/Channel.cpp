#include "Channel.h"
ClassImp(Channel);
Channel::Channel(){}
Channel::~Channel(){}
Channel::Channel(const Channel& other){
    *this = other;
}
Channel::Channel(Channel* other){
    *this = *other;
}