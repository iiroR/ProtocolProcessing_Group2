/*! \file  ControlPlane.hpp
 *  \brief     Header file of ControlPlane module
 *  \details   
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      12.2.2013
 */

/*!
 * \class ControlPlane
 * \brief ControlPlane module runs the BGP process
 *  \details   
 */


#include "systemc"
#include "BGPMessage.hpp"
#include "RoutingTable_Manage_If.hpp"



using namespace std;
using namespace sc_core;
using namespace sc_dt;



#ifndef CONTROLPLANE_H
#define CONTROLPLANE_H




class ControlPlane: public sc_module
{

public:

    
    /*! \brief System clock signal
     * \details The router's internal clock
     * \public
     */
    sc_in_clk port_Clk;
   
    /*! \brief Forwarding port
     * \details Used to write BGP messages to the data plane
     * \public
     */
    sc_port<sc_fifo_out_if<BGPMessage>,1, SC_ZERO_OR_MORE_BOUND> port_ToDataPlane;
   
    /*! \brief Routing Table's management port
     * \details Used to manage the routing table. Add, remove, update routes
     * \public
     */
    sc_port<RoutingTable_Manage_If,1, SC_ZERO_OR_MORE_BOUND> port_RTManage;
   
    /*! \brief Input interface
     * \details Allows data plane to write received BGP messages into
     *  m_ReceivingBuffer-fifo
     * \public
     */
    sc_export<sc_fifo_in_if<BGPMessage> > export_ToControlPlane;
  




  /*! \brief Elaborates the ControlPlane module
   * \details 
   * \public
   */
    ControlPlane(sc_module_name p_ModuleName, int sessions);



  /*! \brief Destructor of the ControlPlane module
   * \details Free's all the dynamically allocated memory 
   * \public
   */
  ~ControlPlane();
  


  /*! \brief The main process of Control Plane module
   * \details \li Reads BGP messages from the m_ReceivingBuffer. \li
   * performs the route resolution process accoriding to BGP protocol.
   * \li Generates the required update messages. \li Keeps track on
   * different BGP sessions.
   * \public
   */
  void controlPlaneMain(void);

  /*! \brief Indicate the systemC producer that this module has a process.
   * \sa http://www.iro.umontreal.ca/~lablasso/docs/SystemC2.0.1/html/classproducer.html
   * \public
   */
  SC_HAS_PROCESS(ControlPlane);




private:
  


    /*! \brief Receiving buffer
     * \details Data plain writes all the received BGP messages into
     * this fifo
     * \private
     */
    sc_fifo<BGPMessage> m_ReceivingBuffer;



  /*! \brief BGP session keepalive timer
   * \details There is one instance for each session. The keepalive
   * timer defines when the next keepalice message is to be sent to
   * the corresponding session.
   * \private
   */
    sc_event **m_BGPSessionKeepalive;



  /*! \brief BGP session hold down timer
   * \details There is one instance of sc_event for each session. If
   * hold down timer expires the link of the corresponding session
   * shall be concidered to be down and the required action need to be taken.
   * \private
   */
    sc_event **m_BGPSessionHoldDown;

    
  /*! \brief Number of BGP sessions
   * \details This defines how many BGP sessions there are in this router
   * \private
   */
    int m_Sessions;
    
  /*! \brief HoldDown time for each session
   * \details Defines the holdDown time for each session. The time is
   * negotiated between the session peers
   * \private
   */
    int *m_HoldDownTime;

};


#endif
