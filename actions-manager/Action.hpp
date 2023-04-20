#ifndef ACTION_COMMAND_HPP_INCLUDED
#define ACTION_COMMAND_HPP_INCLUDED

#include <string>
#include <memory>
#include "action_key.hpp"
#include "BaseArgument.hpp"

//! The Action class implements the "command" design pattern.
/*!
	We use this class to allow users to launch actions from "outside".
    This class is able to receive its parameters through a std::string containing a JSON.
*/
class Action {

protected:

    /**
     *   @brief Represents the execution status of this action. 
     *
     *   We will use the conventions:
     *      -1: Execution hasn't started yet, the object was just instanciated.
     *       0: The execution is succesfully complete.
     *     <-1: An error occured during execution.
     *      >0: The execution is complete but something happened (warning) that's worth to check in the status.
     */
    int statusCode     = -1;

    /// Verbose execution status.
    std::string status = "";

    /// Action's description that will be displayed in the tooltip.
    const std::string description;

public:

    virtual ~Action() {};

    /// Launches the execution of this action. This method must not be overloaded.
    virtual void run() = 0;

    /// Does this instance have all the data it needs to be executed?
    virtual bool poll() = 0;

    /// Takes an object filled with miscellaneous settings and fills this instance's attributes.
    virtual bool assignAll(const rcv::receivedMap& map) = 0;

    /// Takes a string representing a JSON as parameter and fills instance's attributes.
    virtual int  parseArguments(const std::string& rawArguments) = 0;

    /// @return The string representing the execution status of the execution.
    inline const std::string& getStatus() const { return this->status; }

    static const char* action_id() { return ""; }
};


using action_ptr = std::unique_ptr<Action>;

#endif // ACTION_COMMAND_HPP_INCLUDED