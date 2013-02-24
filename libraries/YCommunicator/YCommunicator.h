/*
 * YCommunicator.h
 *
 *  Created on: Jan 11, 2013
 *      Author: oyagev
 */

#ifndef YCommunicator_H_
#define YCommunicator_H_



#include <inttypes.h>
#include <vector>
#include <map>


	class YSerializable {
	public :

		virtual uint8_t getSize() {
			return 0;
		};
		virtual void serialize(uint8_t * buffer) {

		};

		static YSerializable * unserialize( uint8_t * buffer, unsigned int size) {
			YSerializable * s;
			return s;
		};
		virtual ~YSerializable() {

		};
	};

	class YCommPacket : public YSerializable {
		public :
			YSerializable * payload;
			uint16_t checksum;

			YCommPacket(YSerializable * payload);
			void setPayload(YSerializable * payload);

			uint8_t getSize() ;
			void serialize(uint8_t * buffer) ;
			static YCommPacket* unserialize(uint8_t * buffer, unsigned int size) ;

		protected :
			uint16_t calcChecksum(void);

	};

	class YCommInstruction : public YSerializable{
		public :
			uint8_t type;
			uint8_t command;
			uint8_t * data;
			uint8_t data_size;

			const static uint8_t TYPE_CMD 		= 0x00;
			const static uint8_t TYPE_RETURN 	= 0x01;
			const static uint8_t TYPE_ERROR 	= 0xFF;


			YCommInstruction(uint8_t command , uint8_t * data, uint8_t data_size);
			YCommInstruction(uint8_t type, uint8_t command , uint8_t * data, uint8_t data_size);
			uint8_t getSize() ;
			void serialize(uint8_t * buffer) ;
			static YCommInstruction* unserialize(uint8_t * buffer, unsigned int size) ;
	};

	class YCommSerialInputBuffer {
		std::vector<YCommInstruction> instructions;
	public :
		YCommSerialInputBuffer();
		void begin(void);
		void reset(void);
		bool read(uint8_t byte);
		bool read(uint8_t * stream , unsigned int length);
		bool hasInstructions();
		YCommInstruction shiftInstruction();
	private :
		uint8_t * tmp_payload;
		uint16_t tmp_checksum;
		unsigned int expected_inst_size;
		unsigned int tmp_payload_size;
		bool first_byte_received;
		unsigned int tmp_curr_payload_byte;
		uint8_t * tmp_stream;
		unsigned int tmp_curr_byte;
		unsigned int expected_packet_size;
		bool escape_next ;
		std::vector<uint8_t> tmp_buff;



		void buildInstruction();

	};

	class YCommSerialOutputBuffer {
		std::vector<uint8_t> buffer;
	public:
		YCommSerialOutputBuffer();
		void write(YCommInstruction inst);
		void write(YCommPacket packet);
		uint16_t available(void);
		uint8_t shift(void);
	};

	class YCommunicator{
	public:
		YCommunicator();

		void registerDefaultCallback(void(*callbackfunc)(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size));
		void registerCallback(uint8_t command, void(*callbackfunc)(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size));
		void dispatch(YCommInstruction inst);
		void dispatch(uint8_t command, uint8_t * data, uint16_t data_size);
		void dispatch(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size);
		uint16_t available(void);
		uint8_t read(void);
		void write(uint8_t byte);
	protected:
		void (*default_callback)(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size);
		std::map<uint8_t,void (*)(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size)> callbacks_map;

		YCommSerialInputBuffer inp;
		YCommSerialOutputBuffer out;
	};



#endif /* YCommunicator_H_ */
