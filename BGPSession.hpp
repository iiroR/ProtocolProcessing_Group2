/*! \file  BGPSession.hpp
 *  \brief     Header file of BGPSession module
 *  \details   
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      12.2.2013
 */

/*!
 * \class BGPSession
 * \brief BGPSession module handles the HoldDown and Keepalive timers
 * of the session and sends keepalive messages to the session peer
 *  \details BGP session is a sub module of Control Plane. Control
 * Plane has full control on BGP session. First the session is
 * elaborated by Control Plane. Then the session is dedicated for some
 * peer by assigning the peer's BGP identifier to the session. After
 * that the session is started by calling the sessionStart-function.
 * After that the session automatically send the keepalive messages to
 * the peer whenever the keepalive timer expires. Control Plane needs to
 * reset the HoldDown timer whenever it receives a
 * message from the peer. Similarly, whenever Control Plane send a
 * message to the peer, it shall reset the Keepalive timer of the
 * session. The resets is done by calling the functions
 * resetHoldDown and resetKeepalive. Control Plane can track message
 * and the session by comparing the peer's identifier to the one in
 * the session using the isThisSession-function. When ever the
 * HoldDown timer expires the session is stopped automatically.
 * Control plane may check whether the session is still vaid or not
 * using the isSessionValid-function. The checking shall be done
 * before the timers are reset. Whenever Control Plane notices that
 * the session is not valid it shall update the Routing table
 * accordingly and generate required notification messages.
 */


#include "systemc"
#include "BGPMessage.hpp"
#include "BGPSessionParameters.hpp"
#include "DataPlane_In_If.hpp"


using namespace std;
using namespace sc_core;
using namespace sc_dt;

#ifndef _BGPSESSION_H_
#define _BGPSESSION_H_




class BGPSession: public sc_module
{

public:

  



    /*! \brief Output port for BGP messages
     * \details The BGP session writes all the BGP messages to be send
     * to its neighbors into
     * this port. The port shall be bind to the Data Plane's.
     * receiving FIFO
     * \public
     */
    sc_port<DataPlane_In_If> port_ToDataPlane;


    /*! \brief Elaborates the BGPSession module
     * \details 
     * @param[in] sc_module_name p_ModuleName Defines a unique name
     * for this module
     * @param[in] int p_PeeringInterface The outbound interface to
     * which the peer connects
     * @param[in] BGPSessionParameters p_SessionParameters Holds the
     * keepalive fraction, holddown time, etc. values for this session
     * \public
     */
    BGPSession(sc_module_name p_ModuleName, int p_PeeringInterface, BGPSessionParameters p_SessionParam);

    /*! \brief Elaborates the BGPSession module
     * \details 
     * @param[in] sc_module_name p_ModuleName Defines a unique name
     * for this module
     * @param[in] BGPSessionParameters p_SessionParameters Holds the
     * keepalive fraction, holddown time, etc. values for this session
     * \public
     */
    BGPSession(sc_module_name p_ModuleName, BGPSessionParameters p_SessionParam);



    /*! \brief Destructor of the BGPSession module
     * \details Free's all the dynamically allocated memory 
     * \public
     */
    ~BGPSession();
  


    /*! \brief Send a keepalive message to the peer
     * \details A SystemC method, which is sensitive to m_BGPKeepalive event
     * \public
     */
    void sendKeepalive(void);

    /*! \brief Invalidates this session
     * \details A SystemC method, which is sensitive to m_BGPHoldDown event
     * \public
     */
    void sessionInvalidation(void);




    /*! \brief Sets the BGP session parameters for this session
     * \details 
     * @param[in] BGPSessionParameters p_SessionParameters Holds the
     * keepalive fraction, holddown time, etc. values
     * \public
     */
    void setSessionParameters(BGPSessionParameters p_SessionParam);

    /*! \brief Resets the HoldDown timer
     * \details Allows the control plane to reset the HoldDown timer
     * whenever a message from the session peer is received
     * \public
     */
    void resetHoldDown(void);

    /*! \brief Checks whether this session is valid or not
     * \details Allows the control plane to check whether this session
     * is still valid or not. I.e. is the HoldDown timer expired.
     * \public
     */
    bool isSessionValid(void);

    /*! \brief Stops this session
     * \details HoldDown and Keepalive timers are stopped and no
     * keepalive messages are sent after a call of this function
     * \public
     */
    void sessionStop(void);

    /*! \brief Starts the session
     * \details HoldDown and Keepalive timers are reset and the
     * sending of keepalive messages starts
     * \public
     */
    void sessionStart(void);

    /*! \brief Sets the BGP Identifier of the session peer
     * \details
     * @param[in] sc_int<32> p_BGPIdentifier of the session peer
     * received message
     * \public
     */
    void setPeerIdentifier(sc_int<32> p_BGPIdentifier);

    /*! \brief Checks whether this session is for the passed BGP Identifier
     * \details
     * @param[in] sc_int<32> p_BGPIdentifier The BGP Identifier of the
     * received message
     * \return <bool> True: if this session corresponds the received
     * message a identifier. False: in any other case
     * \public
     */
    bool isThisSession(sc_int<32> p_BGPIdentifier);

    /*! \brief Resets the Keepalive timer
     * \details 
     * \public
     */
    void resetKeepalive(void);

    /*! \brief Indicate the systemC producer that this module has a process.
     * \sa http://www.iro.umontreal.ca/~lablasso/docs/SystemC2.0.1/html/classproducer.html
     * \public
     */
    SC_HAS_PROCESS(BGPSession);




private:
  


    /*! \brief Handles the arbitration for Keepalive reset
     * \details Keepalive can be reset either internally by the
     * session or externally by the Control Plane.
     * \private
     */
    sc_mutex m_KeepaliveMutex;

    /*! \brief BGP session keepalive timer
     * \details There is one instance for each session. The keepalive
     * timer defines when the next keepalice message is to be sent to
     * the corresponding session.
     * \private
     */
    sc_event m_BGPKeepalive;



    /*! \brief BGP session hold down timer
     * \details There is one instance of sc_event for each session. If
     * hold down timer expires the link of the corresponding session
     * shall be concidered to be down and the required action need to be taken.
     * \private
     */
    sc_event m_BGPHoldDown;

    /*! \brief Interface of the Session Peer
     * \details Index of the Interface of this router to which the
     * peer of this session connects
     * \private
     */
    int m_PeeringInterface;
    
    /*! \brief HoldDown time for this session
     * \details Defines the holdDown time for this session. The default
     * value needs to be set in the elaboration phase. After that the
     * BGP session may negotiated a new value between the session peers
     * \private
     */
    int m_HoldDownTime;
     
    /*! \brief Keepalive time for this session
     * \details Defines interval for sending keepalive messages between
     * the peers of this session.
     * \private
     */
    int m_KeepaliveTime;


    /*! \brief Keepalive fraction
     * \details Defines the divider that is used while determining the
     * keepalive interval 
     * \private
     */
    int m_KeepaliveFraction;

    /*! \brief Indicates whether this session is valid or not
     * \details Is set to True when the session start. When ever the
     * HoldDown timer expires the value of m_SessionValidity shall be
     * set to Fasle
     * \private
     */
    bool m_SessionValidity;


    /*! \brief BGP message object
     * \details This holds the keepalive messages
     * \private
     */
    BGPMessage m_KeepaliveMsg;

    sc_int<32> m_BGPIdentifierPeer;

    /***************************Private functions*****************/




};




#endif /* _BGPSESSION_H_ */
