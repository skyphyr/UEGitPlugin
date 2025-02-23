// Copyright (c) 2014-2020 Sebastien Rombauts (sebastien.rombauts@gmail.com)
//
// Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
// or copy at http://opensource.org/licenses/MIT)

#pragma once

#include "CoreMinimal.h"
#include "ISourceControlProvider.h"
#include "Misc/EngineVersionComparison.h"
#include "Runtime/Launch/Resources/Version.h"

struct FToolMenuSection;
class FMenuBuilder;

/** Git extension of the Revision Control toolbar menu */
class FGitSourceControlMenu
{
public:
	void Register();
	void Unregister();
	
	/** This functions will be bound to appropriate Command. */
	void CommitClicked();
	void PushClicked();
	void SyncClicked();
	void RevertClicked();
	void RefreshClicked();

protected:
	static void RevertAllCallback(const FSourceControlOperationRef& InOperation, ECommandResult::Type InResult);
	static void RevertAllCancelled(FSourceControlOperationRef InOperation);

private:
	bool HaveRemoteUrl() const;

	bool SaveDirtyPackages();

	bool StashAwayAnyModifications();
	void ReApplyStashedModifications();

#if !UE_VERSION_OLDER_THAN(5, 0, 0)
	void AddMenuExtension(FToolMenuSection& Builder);
#else
	void AddMenuExtension(FMenuBuilder& Builder);
	TSharedRef<class FExtender> OnExtendLevelEditorViewMenu(const TSharedRef<class FUICommandList> CommandList);
#endif

	static void DisplayInProgressNotification(const FText& InOperationInProgressString);
	static void RemoveInProgressNotification();
	static void DisplaySuccessNotification(const FName& InOperationName);
	static void DisplayFailureNotification(const FName& InOperationName);

#if UE_VERSION_OLDER_THAN(5, 0, 0)
	FDelegateHandle ViewMenuExtenderHandle;
#endif

	/** Was there a need to stash away modifications before Sync? */
	bool bStashMadeBeforeSync = false;

	/** Loaded packages to reload after a Sync or Revert operation */
	TArray<UPackage*> PackagesToReload = TArray<UPackage*>();

	/** Current revision control operation from extended menu if any */
	static TWeakPtr<class SNotificationItem> OperationInProgressNotification;

	/** Delegate called when a revision control operation has completed */
	void OnSourceControlOperationComplete(const FSourceControlOperationRef& InOperation, ECommandResult::Type InResult);
};
