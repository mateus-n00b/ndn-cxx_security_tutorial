/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2019 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Alexander Afanasyev <http://lasr.cs.ucla.edu/afanasyev/index.html>
 */

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/validator-config.hpp>
#include <ndn-cxx/security/validation-error.hpp>
#include <iostream>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces should be used to prevent/limit name conflicts
namespace examples {
// apt-get install psmisc
class Consumer
{
public:
  Consumer(const std::string &fileName)
  {

      m_validator = make_shared<ValidatorConfig>(m_face);

      try {
        std::cout << "OPEN File= " << fileName << std::endl;
        m_validator->load(fileName);
      }
      catch (const std::exception &e ) {
        std::cout << "Can not load File= " << fileName << ". Error: " << e.what()
          <<  std::endl;
        exit(1);
      }

  }

  void
  run()
  {
    Name interestName("/example/testApp/randomData");
    interestName.appendVersion();

    Interest interest(interestName);
    interest.setCanBePrefix(false);
    interest.setMustBeFresh(true);
    interest.setInterestLifetime(6_s); // The default is 4 seconds

    std::cout << "Sending Interest " << interest << std::endl;
    m_face.expressInterest(interest,
                           bind(&Consumer::onData, this,  _1, _2),
                           bind(&Consumer::onNack, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    // processEvents will block until the requested data is received or a timeout occurs
    m_face.processEvents();
  }

private:
  void
  onData(const Interest&, const Data& data) const
  {
    std::cout << "Received Data " << data << std::endl;

    // Validating data
    m_validator->Validator::validate(data,
                         bind(&Consumer::onValidated, this, _1),
                         bind(&Consumer::onValidationFailed, this, _1, _2));
  }

  void
  onValidated(const Data& data) const
  {
    std::cout << "Validated data: " << data.getName() << std::endl;
  }

  void
  onValidationFailed(const Data& data, const ndn::security::v2::ValidationError& failureInfo) const
  {
    std::cerr << "Not validated data: " << data.getName()
              << ". The failure info: " << failureInfo << std::endl;
  }

  void
  onNack(const Interest&, const lp::Nack& nack) const
  {
    std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
  }

  void
  onTimeout(const Interest& interest) const
  {
    std::cout << "Timeout for " << interest << std::endl;
  }

private:
  Face m_face;
  shared_ptr<ndn::ValidatorConfig> m_validator;
};

} // namespace examples
} // namespace ndn

int
main(int argc, char** argv)
{
 if (argc != 2) {
     std::cerr << "Validation file must be specified" << std::endl;
     std::cerr << "General use:" << std::endl;
     std::cerr << "  " << argv[0] << " validation_file" << std::endl;
     return 1;
  }

  ndn::examples::Consumer consumer(argv[1]);
  try {
    consumer.run();
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
  return 0;
}
