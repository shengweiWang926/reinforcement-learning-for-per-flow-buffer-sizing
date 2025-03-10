/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Pasquale Imputato <p.imputato@gmail.com>
 *          Stefano Avallone <stefano.avallone@unina.it>
 */

#ifndef RL_QUEUE_DISC
#define RL_QUEUE_DISC

#include "ns3/queue-disc.h"
#include "ns3/object-factory.h"
#include "ns3/rl-net.h"
#include <list>
#include <map>

namespace ns3 {

/**
 * \ingroup traffic-control
 *
 * \brief A flow queue used by the RL queue disc
 */

class RLFlow : public QueueDiscClass {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief RLFlow constructor
   */
  RLFlow ();

  virtual ~RLFlow ();

  /**
   * \enum FlowStatus
   * \brief Used to determine the status of this flow queue
   */
  enum FlowStatus
    {
      INACTIVE,
      NEW_FLOW,
      OLD_FLOW
    };

  /**
   * \brief Set the deficit for this flow
   * \param deficit the deficit for this flow
   */
  void SetDeficit (uint32_t deficit);
  /**
   * \brief Get the deficit for this flow
   * \return the deficit for this flow
   */
  int32_t GetDeficit (void) const;
  /**
   * \brief Increase the deficit for this flow
   * \param deficit the amount by which the deficit is to be increased
   */
  void IncreaseDeficit (int32_t deficit);
  /**
   * \brief Set the status for this flow
   * \param status the status for this flow
   */
  void SetStatus (FlowStatus status);
  /**
   * \brief Get the status of this flow
   * \return the status of this flow
   */
  FlowStatus GetStatus (void) const;

private:
  int32_t m_deficit;    //!< the deficit for this flow
  FlowStatus m_status;  //!< the status of this flow
};


/**
 * \ingroup traffic-control
 *
 * \brief A RL packet queue disc
 */

class RLQueueDisc : public QueueDisc {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief RLQueueDisc constructor
   */
  RLQueueDisc ();

  virtual ~RLQueueDisc ();

   /**
    * \brief Set the quantum value.
    *
    * \param quantum The number of bytes each queue gets to dequeue on each round of the scheduling algorithm
    */
   void SetQuantum (uint32_t quantum);

   /**
    * \brief Get the quantum value.
    *
    * \returns The number of bytes each queue gets to dequeue on each round of the scheduling algorithm
    */
   uint32_t GetQuantum (void) const;

  void SetRlNet (std::shared_ptr<ns3::RlNet>  rl_net);
  void SetBw(double bw);
  void SetDelay(double delay);
  std::shared_ptr<ns3::RlNet>  GetRlNet (void) const;

  double GetSummedQueueLength() const;
  double GetSummedMaxQueueLength() const;
  double GetSentBytes() const;
  double GetSummedQueueLengthUntilDecision() const;
  double GetSentBytesUntilDecision() const;
  int8_t GetActualDecision() const;
  uint32_t GetAlreadyInspected() const;
  torch::Tensor GetActualInput() const;
  torch::Tensor GetPrediction() const;
  double GetActualTime() const;
  int GetChosenQueueSize() const;

  // Reasons for dropping packets
  static constexpr const char* UNCLASSIFIED_DROP = "Unclassified drop";  //!< No packet filter able to classify packet
  static constexpr const char* OVERLIMIT_DROP = "Overlimit drop";        //!< Overlimit dropped packets
  double m_time_for_action = -1;
  int32_t m_action_to_perform = -1;

private:
  virtual bool DoEnqueue (Ptr<QueueDiscItem> item);
  virtual Ptr<QueueDiscItem> DoDequeue (void);
  virtual bool CheckConfig (void);
  virtual void InitializeParams (void);

  /**
   * \brief Drop a packet from the head of the queue with the largest current byte count
   * \return the index of the queue with the largest current byte count
   */
  uint32_t RLDrop (void);

  std::shared_ptr<ns3::RlNet> m_rl_net;
  double m_bw;
  double m_delay;
  uint32_t m_quantum;        //!< Deficit assigned to flows at each round
  uint32_t m_flows;          //!< Number of flow queues
  uint32_t m_dropBatchSize;  //!< Max number of packets dropped from the fat flow
  uint32_t m_perturbation;   //!< hash perturbation value

  std::list<Ptr<RLFlow> > m_newFlows;    //!< The list of new flows
  std::list<Ptr<RLFlow> > m_oldFlows;    //!< The list of old flows

  std::map<uint32_t, uint32_t> m_flowsIndices;    //!< Map with the index of class for each flow

  ObjectFactory m_flowFactory;         //!< Factory to create a new flow
  ObjectFactory m_queueDiscFactory;    //!< Factory to create a new queue
};

} // namespace ns3

#endif /* RL_QUEUE_DISC */
