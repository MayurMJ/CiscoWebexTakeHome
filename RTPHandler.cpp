
#include <iostream>
#include <assert.h>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

// The RTP packet format is described at
// https://tools.ietf.org/html/rfc3550#page-13

typedef std::function<void(uint32_t)> rtp_callback_fn;
class RtpHandler {

private:

   // rtp packet structure with all the fields
    struct rtp{
        unsigned int v : 2;  // version
        unsigned int p : 1;  // padding
        unsigned int x : 1;  // extension
        unsigned int cc: 4;  // size of csrc
        unsigned int m: 1;   // marking
        unsigned int pt: 7;  // payload type
        uint16_t seq;        // Sequence Number
        uint32_t timestamp;  // timestamp
        uint32_t ssrc;       // synchronization source
        std::vector<uint32_t> csrc; // contributing sources
        std::vector<uint32_t> ext; // extension
        uint16_t defP = 0;    // defined by profile
        uint16_t extLength = 0; // length of extension header
    };
	
	// unordered_map which stores all csrc callbacks
    std::unordered_map<uint32_t, rtp_callback_fn> m_callbacks;


    /*
     *  Parses the entire rtp Packet and extracts the Header.
     *
     */

    std::unique_ptr<rtp> parsePacket(const uint8_t *rtpPacket) const {
        std::unique_ptr<rtp> p(new rtp());
        uint8_t oct = rtpPacket[0];
        p->cc = oct & 0x0f;
        oct = oct >> 4;
        p->x = oct & 1;
        oct = oct >> 1;
        p->p = oct & 1;
        oct = oct >> 1;
        p->v = oct & 2;
        oct = rtpPacket[1];
        p->pt = 0x7f & oct;
        oct = oct >> 7;
        p->m = oct & 1;
        p->seq = (rtpPacket[2] << 8) | rtpPacket[3]; 
        p->timestamp = (rtpPacket[4] << 24) | (rtpPacket[5] << 16) |
                       (rtpPacket[6] << 8) | rtpPacket[7];
        p->ssrc = (rtpPacket[8] << 24) | (rtpPacket[9] << 16) |
                  (rtpPacket[10] << 8) | rtpPacket[11];
       
        int headerIndex = 12;
        for(int i = 0; i < p->cc; i++) {
            p->csrc.push_back((rtpPacket[headerIndex] << 24) | (rtpPacket[headerIndex+1] << 16) |
                              (rtpPacket[headerIndex+2] << 8) | rtpPacket[headerIndex+3]);
            headerIndex +=4;
        }
        if(p->x == 1) {
            p->defP = rtpPacket[headerIndex] << 8 | rtpPacket[headerIndex + 1];
            headerIndex += 2;
            p->extLength = rtpPacket[headerIndex] << 8 | rtpPacket[headerIndex + 1];
            headerIndex +=2;
            for(int i = 0; i < p->extLength; i++) {
                p->ext.push_back((rtpPacket[headerIndex] << 24) | (rtpPacket[headerIndex+1] << 16) |
                                 (rtpPacket[headerIndex+2] << 8) | rtpPacket[headerIndex+3]);
                headerIndex +=4;
            }
        }
        return std::move(p);
    }

    
    /*
     *  Prints the packet, used for debug
     *
     */

    void printRTP(std::unique_ptr<rtp> &p) const{
        std::cout << std::hex << "v: " << p->v;
        std::cout << std::hex << " p: " << p->p;
        std::cout << std::hex << " x: " << p->x;
        std::cout << std::hex << " cc: " << p->cc;
        std::cout << std::hex << " m: " << p->m;
        std::cout << std::hex << " pt: " << p->pt;
        std::cout << std::hex << " seq: " << p->seq;
        std::cout << std::hex << " timestamp: " << p->timestamp;
        std::cout << std::hex << " ssrc: " << p->ssrc << std::endl;
        for(int i = 0; i < p->cc; i++) {
            std::cout << std::hex << " csrc: " << p->csrc[i];
        }
        std::cout << std::endl;
        if(p->x) {
            for(int i = 0; i < p->extLength; i++) {
                std::cout << std::hex << " ext: " << p->ext[i];
            }
        }
        std::cout << std::endl;
    }
    
    
public:
   // register callback. when packet with matching csrc is
   // received, callback is called
   void registerCallback(uint32_t csrc,
                         rtp_callback_fn callback) {
      // TODO 1: add implementation
      m_callbacks[csrc] = callback;
   }

   // receive RTP packet, and call any matching callback
  void receivePacket(const uint8_t*rtpPacket) const {
     // TODO 2: add implementation
     std::unique_ptr<rtp> p = parsePacket(rtpPacket);
     //printRTP(p);
     for(int i = 0; i < p->cc; i++) {
         auto it = m_callbacks.find(p->csrc[i]);  
         if( it != m_callbacks.end()) {
             it->second(p->csrc[i]);
         }
     }
  }
};


const uint8_t packet1[] = {0x91, 0x6c, 0xd3, 0x31, 0x48, 0x0d, 0xb8, 0x9a, 0x10, 0x58, 0xf3, 0xf2,
 0x84, 0x2a, 0xbb, 0x01, 0xbe, 0xde, 0x00, 0x01, 0x10, 0x28, 0x20, 0x00,
 0x61, 0x01, 0x43, 0x20, 0xbb, 0x42, 0xed, 0x0f, 0x90, 0x1d, 0xe0, 0xa6};
const uint8_t packet2[] = {0x91, 0x6c, 0x57, 0xb4, 0xfc, 0xaa, 0x7e, 0x3e, 0x29, 0x4d, 0x07, 0x6f,
0xe8,
 0x32, 0x8f, 0x01, 0xbe, 0xde, 0x00, 0x01, 0x10, 0x1e, 0x20, 0x80, 0x01, 0x02,
 0x8c, 0x70, 0x02, 0x20, 0x08, 0x67, 0x20, 0x67, 0x89, 0xeb, 0xd0, 0x24, 0x40};
const uint8_t packet3[] = {0x91, 0x6c, 0xa4, 0x66, 0xe3, 0x8e, 0xbf, 0x36, 0xdf, 0x47, 0x72, 0x57,
0xe9,
 0xcf, 0x61, 0x01, 0xbe, 0xde, 0x00, 0x01, 0x10, 0x32, 0x20, 0x00, 0x61, 0xd0,
 0xba, 0x62, 0xe9, 0x8f, 0x90, 0x1d, 0xe0, 0x45, 0xe6, 0xbd, 0xe3, 0x20};
const uint8_t packet4[] = {0x90, 0x6c, 0xa4, 0x66, 0xe3, 0x8e, 0xbf, 0x36, 0xdf, 0x47, 0x72, 0x01,
 0xbe, 0xde, 0x00, 0x01, 0x10, 0x32, 0x20, 0x00, 0x61, 0xd0, 0xba, 0x62,
 0xe9, 0x8f, 0x90, 0x1d, 0xe0, 0x45, 0xe6, 0xbd, 0xe3, 0x20};


int main(int argc, char** argv) {
 
    // a map of CSRC to a count of the number of times the CSRC was seen
    std::map<uint32_t, std::size_t> csrcToCountMap;
    RtpHandler handler;
    
    // TODO 3: add callbacks for the following CSRC
    auto callback = [&](uint32_t val) { csrcToCountMap[val]++;};

    handler.registerCallback(0x1058f3f2, callback);
    handler.registerCallback(0x842abb01, callback);
    handler.registerCallback(0xe8328f01, callback);

    handler.receivePacket(packet1);
    handler.receivePacket(packet2);
    handler.receivePacket(packet3);
    handler.receivePacket(packet4);

    // the following expressions should all be true, so no assert is triggered
    assert(csrcToCountMap.size() == 2);
    assert(csrcToCountMap.count(0x1058f3f2) == 0);
    assert(csrcToCountMap.count(0x842abb01) == 1);
    assert(csrcToCountMap.count(0xe8328f01) == 1);
}
RtpHandler handler;
