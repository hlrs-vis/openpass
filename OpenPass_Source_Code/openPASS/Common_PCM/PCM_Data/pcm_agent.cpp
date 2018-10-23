#include "pcm_agent.h"

PCM_Agent::PCM_Agent(int id):
    id(id)
{}

int PCM_Agent::GetId() const
{
    return id;
}
