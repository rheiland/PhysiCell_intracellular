#ifndef _RoadRunner_Intracellular_h_
#define _RoadRunner_Intracellular_h_

#include <string>
#include <map>
#include <iomanip>   // for setw

#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../modules/PhysiCell_pugixml.h"
// #include "maboss_network.h"

// #ifdef ADDON_ROADRUNNER
// These are for C
// #define STATIC_RRC
#include "rrc_api.h"
#include "rrc_types.h"
// #include "rrc_utilities.h"
extern "C" rrc::RRHandle createRRInstance();
// #endif

class RoadRunnerIntracellular : public PhysiCell::Intracellular {
 private:
 public:
	
	static long counter;
	
	double time_step = 12;
	bool discrete_time = false;
	double time_tick = 0.5;
	double scaling = 1.0;
	
	std::map<std::string, double> initial_values;
	std::map<std::string, double> parameters;
	
    // rrc::RRHandle rrHandle = createRRInstance();
    rrc::RRHandle rrHandle;
    // rrc::RRHandle rrHandle;
    // rrc::RRVectorPtr vptr;
	rrc::RRCDataPtr result;  // start time, end time, and number of points

	double next_librr_run = 0;

    RoadRunnerIntracellular();

	RoadRunnerIntracellular(pugi::xml_node& node);
	
	RoadRunnerIntracellular(RoadRunnerIntracellular* copy);
	
	Intracellular* clone() {
		return static_cast<Intracellular*>(new RoadRunnerIntracellular(this));
	}
	Intracellular* getIntracellularModel() 
    {
        std::cout << "------ librr_intracellular: getIntracellularModel called\n";
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
    // Need 'int' return type to avoid bizarre compile errors.
	int start();

    // Need 'int' return type to avoid bizarre compile errors.
	int update();
	
	bool need_update() {
		return PhysiCell::PhysiCell_globals.current_time >= this->next_librr_run;
	}
	
	double get_parameter_value(std::string name) {
		// return this->maboss.get_parameter_value(name);
		// return this->rrHandle.get_parameter_value(name);
        return 42.0;
	}
	
	int set_parameter_value(std::string name, double value) {
		// this->maboss.set_parameter_value(name, value);
        return 0;
	}
	
	std::string get_state() {
		// return this->maboss.get_state();
        // const std::string s0 ( "bogus get_state" );
        // return s0;
        return this->sbml_file;
	}

    // for now, define dummy methods for these in the abstract parent class
    bool has_node(std::string name) { return false; }
	bool get_boolean_node_value(std::string name) { return false; }
	void set_boolean_node_value(std::string name, bool value)  {}
    void print_current_nodes() {}
	
	// static void save_PhysiBoSS(std::string path, std::string index);
	static void save_libRR(std::string path, std::string index);

};

#endif