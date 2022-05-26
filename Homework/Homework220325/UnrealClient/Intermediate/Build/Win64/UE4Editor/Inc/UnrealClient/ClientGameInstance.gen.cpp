// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UnrealClient/Global/ClientGameInstance.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeClientGameInstance() {}
// Cross Module References
	UNREALCLIENT_API UClass* Z_Construct_UClass_UClientGameInstance_NoRegister();
	UNREALCLIENT_API UClass* Z_Construct_UClass_UClientGameInstance();
	ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
	UPackage* Z_Construct_UPackage__Script_UnrealClient();
// End Cross Module References
	void UClientGameInstance::StaticRegisterNativesUClientGameInstance()
	{
	}
	UClass* Z_Construct_UClass_UClientGameInstance_NoRegister()
	{
		return UClientGameInstance::StaticClass();
	}
	struct Z_Construct_UClass_UClientGameInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UClientGameInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UGameInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_UnrealClient,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UClientGameInstance_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Global/ClientGameInstance.h" },
		{ "ModuleRelativePath", "Global/ClientGameInstance.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UClientGameInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UClientGameInstance>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UClientGameInstance_Statics::ClassParams = {
		&UClientGameInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A8u,
		METADATA_PARAMS(Z_Construct_UClass_UClientGameInstance_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UClientGameInstance_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UClientGameInstance()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UClientGameInstance_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UClientGameInstance, 3063997466);
	template<> UNREALCLIENT_API UClass* StaticClass<UClientGameInstance>()
	{
		return UClientGameInstance::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UClientGameInstance(Z_Construct_UClass_UClientGameInstance, &UClientGameInstance::StaticClass, TEXT("/Script/UnrealClient"), TEXT("UClientGameInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UClientGameInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
