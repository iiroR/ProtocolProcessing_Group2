/*! \file ControlPlane.cpp
 *  \brief     Implementation of ControlPlane.
 *  \details   
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      Tue Feb 12 12:32:09 2013
 */


#include "ControlPlane.hpp"


ControlPlane::ControlPlane(sc_module_name p_ModName, int p_Sessions, BGPSessionParameters p_BGPParameters):sc_module(p_ModName)
{
  //make the inner bindings
    export_ToControlPlane(m_ReceivingBuffer); //export the receiving
                                             //buffer's input
                                             //interface for the data plane


    //set the session count
    m_SessionCount = p_Sessions;

    //initiate the BGPSession pointer arrays
    m_BGPSessions = new BGPSession*[m_SessionCount];

    //inititate the sessions
    for (int i = 0; i < m_SessionCount; ++i)
        {
            //create a session for the peer behind interface i
            m_BGPSessions[i] = new BGPSession("BGP_Session", i, p_BGPParameters);
        }
    
    SC_THREAD(controlPlaneMain);
    sensitive << port_Clk.pos();
}

ControlPlane::~ControlPlane()
{

    for (int i = 0; i < m_SessionCount; ++i)
        delete m_BGPSessions[i];
    delete m_BGPSessions;
}


void ControlPlane::controlPlaneMain(void)
{
  cout << name() << " starting at time" << sc_time_stamp()  << endl;

  cout << name() << " starts the sessions"  << endl;

    //start the sessions
    for (int i = 0; i < m_SessionCount; ++i)
        {
            //create a session for the peer behind interface i
            m_BGPSessions[i]->sessionStart();
        }

    while(true)
    {
        wait();

        //Check if there's messages in the input buffer
      if(m_ReceivingBuffer.num_available() > 0)
          {

              //Handle the message here
          }



      //verify the session validity
      for (int i = 0; i < m_SessionCount; ++i)
          {
              //
              if (!m_BGPSessions[i]->isSessionValid())
                  {
                   
                      //remove the routes behind the interface i
                      
                  }

              
          }
      
    
    }
}

