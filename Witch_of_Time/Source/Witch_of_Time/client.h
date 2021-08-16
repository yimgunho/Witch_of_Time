#pragma once

#include "CoreMinimal.h"
#include "BlockBase.h"
#include "../../../Server/Server/TCPServer.h"
#include <string>
#include <vector>
#include <thread>
#include "GameFramework/Actor.h"
#include "client.generated.h"
#define MAX_SOCKET 10 

USTRUCT(BlueprintType)
struct FPlayerinfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	AActor* playeractor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	FVector player_pos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	FRotator player_ang;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		int player_hp = 100;
};

USTRUCT(BlueprintType)
struct FCommandBlockArray
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	int blockid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	TArray<FCommandBlockInfo> MyActors;
};

UCLASS()
class WITCH_OF_TIME_API Aclient : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	//전역 변수로 선언하지 말고 여기 선언하고 변수이름 좀더 잘 알아볼수 있게 만들기
	Aclient();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FCommandBlockArray> commandblockarr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempSendStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempRecvStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int recv_cnt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockindex_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* ToDestroyBlock_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ToDestroyBlock_cnt = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString ToDestroyBlockName_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString ToDestroyBlockName_CL_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Block_cnt = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Block_cnt_load = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_index = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int todestroyblockid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int todestroyblockid_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_id_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_id_CL_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString Chatting_FString;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool is_changed_mode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int playerindex_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_x_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_y_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_z_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_x_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_y_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_z_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempCommandBlockName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int lengthofcommandlist;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockindex_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_0_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_1_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_2_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> commandblockdata_3_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempCommandBlockName_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TempCommandBlockId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TempCommandBlockId_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool is_ready;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ready_switch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int is_all_ready;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_x_load;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_y_load;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_z_load;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int blockindex_load;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> id_arr_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_x_arr_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_y_arr_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_z_arr_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> blockindex_arr_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> id_arr_to_levelEditor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_x_arr_to_levelEditor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_y_arr_to_levelEditor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_z_arr_to_levelEditor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> blockindex_arr_to_levelEditor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString FastTimeBlock_name_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString SlowTimeBlock_name_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TimeBlock_id_Gen;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int SlowTimeBlock_id_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int FastTimeBlock_id_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TimeBlock_id_SERVER;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TimeBlock_type_SERVER;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FPlayerinfo> players;




	UFUNCTION(BlueprintCallable)
		void SetBlockIndex(int index);

	UFUNCTION(BlueprintCallable)
	void send_destroy_packet(int block_id);

	UFUNCTION(BlueprintCallable)
		void send_attack_packet(int block_id);

	UFUNCTION(BlueprintCallable)
	void send_chatting_packet(FString chat);

	UFUNCTION(BlueprintCallable)
	void send_block_packet(int blockindex, float block_pos_x, float block_pos_y, float block_pos_z);

	UFUNCTION(BlueprintCallable)
	void send_block_with_command_packet(int blockindex, float block_pos_x, float block_pos_y, float block_pos_z,
		TArray<int32> commandindex, TArray<float> commanddata_0, TArray<float> commanddata_1, TArray<float> commanddata_2, TArray<float> commanddata_3);

	UFUNCTION(BlueprintCallable)
	void send_change_packet();

	UFUNCTION(BlueprintCallable)
		void send_playerinfo_packet(int hp);

	UFUNCTION(BlueprintCallable)
		void send_player_packet(FVector player_pos, FRotator player_angle);

	UFUNCTION(BlueprintCallable)
		void send_time_packet();

	UFUNCTION(BlueprintCallable)
		void send_command_packet(int block_id, TArray<int32> commandindex, TArray<float> commanddata_0, TArray<float> commanddata_1, TArray<float> commanddata_2, TArray<float> commanddata_3);

	UFUNCTION(BlueprintImplementableEvent)
		void apply_damage(AActor* actor, int hp, bool flag = false);

	UFUNCTION(BlueprintImplementableEvent)
		void move_player(float x, float y, float z, float a_x, float a_y, float a_z);

	UFUNCTION(BlueprintImplementableEvent)
		void mode_change();

	UFUNCTION(BlueprintImplementableEvent)
		void spawn_dummy_player(int index, FVector pos, FRotator ang);

	UFUNCTION(BlueprintImplementableEvent)
	void spawn_block(int index, int id, float x, float y, float z);

	UFUNCTION(BlueprintImplementableEvent)
	void spawn_with_command_block(int index, int id, float x, float y, float z, const TArray<int32>& commandindex, const TArray<float>& data_0, const TArray<float>& data_1, const TArray<float>& data_2, const TArray<float>& data_3);

	UFUNCTION(BlueprintImplementableEvent)
	void casting_magic(int block_id, int magic_type);

	UFUNCTION(BlueprintImplementableEvent)
	void destroy_block(int id);

	UFUNCTION(BlueprintImplementableEvent)
	void paste_commandlist(int id, const TArray<int32>& index, const TArray<float>& data_0, const TArray<float>& data_1, const TArray<float>& data_2, const TArray<float>& data_3);

	UFUNCTION(BlueprintImplementableEvent)
	void receive_chatting();

	UFUNCTION(BlueprintImplementableEvent)
		void attack_monster(int block_id);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int my_index = -1;

	std::thread *worker_thread;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//static DWORD WINAPI SendThread(void* lpData);
public:
	void worker();
	void process_packet(int p_id, unsigned char* p_buf);
	void send_packet(void* p);
	void do_recv(int s_id);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
