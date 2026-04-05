// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/TeamInterface.h"

// Add default functionality here for any ITeamInterface functions that are not pure virtual.

ETeam ITeamInterface::GetTeam_Implementation() const
{
    return ETeam::Neutral;
}
