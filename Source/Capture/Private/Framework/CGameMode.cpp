// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameMode.h"

APlayerController* ACGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewController = Super::SpawnPlayerController(InRemoteRole, Options);
	IGenericTeamAgentInterface* ControllerTeamInterface = Cast<IGenericTeamAgentInterface>(NewController);
	FGenericTeamId TeamId = GetTeamIdForPlayer(NewController);
	if (ControllerTeamInterface)
	{
		ControllerTeamInterface->SetGenericTeamId(TeamId);
	}
	
	return NewController;
}

FGenericTeamId ACGameMode::GetTeamIdForPlayer(const APlayerController* PlayerController)
{
	static int PlayerCount = 0;
	++PlayerCount;
	
	return FGenericTeamId(PlayerCount%2);
}
