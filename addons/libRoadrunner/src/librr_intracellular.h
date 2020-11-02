#ifndef _SBML_Intracellular_h_
#define _SBML_Intracellular_h_

#include <string>
#include <map>
#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../modules/PhysiCell_pugixml.h"
// #include "maboss_network.h"

// #ifdef ADDON_SBML
// These are for C
// #define STATIC_RRC
#include "rrc_api.h"
#include "rrc_types.h"
// #include "rrc_utilities.h"
extern "C" rrc::RRHandle createRRInstance();
// #endif

class SBMLIntracellular : public PhysiCell::Intracellular {
 private:
 public:
	
	static long counter;
	
	// std::string sbml_filename;
	
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

    SBMLIntracellular();

	SBMLIntracellular(pugi::xml_node& node);
	
	SBMLIntracellular(SBMLIntracellular* copy);
	
	Intracellular* clone() {
		return static_cast<Intracellular*>(new SBMLIntracellular(this));
	}
	Intracellular* getIntracellularModel() 
    {
        std::cout << "------ librr_intracellular: getIntracellularModel called\n";
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
    // Need 'int' return type to avoid bizarre compile errors.
	// void start() 
	int start() 
    {
        std::cout << "------ librr_intracellular.h: start() called\n";
		// this->maboss.restart_node_values();
        this->rrHandle = createRRInstance();

        // if (!rrc::loadSBML (rrHandle, get_cell_definition("lung epithelium").sbml_filename.c_str())) 
        if (!rrc::loadSBML(rrHandle, "./config/Toy_SBML_Model_1.xml") )
        {
		    std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
	        // 	printf ("Error message: %s\n", getLastError());
	        // 	getchar ();
	        // 	exit (0);
	    }

        std::cout << "----- start(): rrHandle=" << rrHandle << std::endl;
        return 42;
	}

    // Need 'int' return type to avoid bizarre compile errors.
	// void update() 
	int update() 
    {
		// this->maboss.run_simulation();
		// (this->rrHandle).run_simulation();
        // result = rrc::simulateEx (pCell->phenotype.molecular.model_rr, 0, 10, 10);  // start time, end time, and number of points
        std::cout << "----- update(): rrHandle=" << this->rrHandle << std::endl;
        result = rrc::simulateEx (this->rrHandle, 0, 10, 10);  // start time, end time, and number of points
		// this->next_librr_run += this->rrHandle.get_time_to_update();
        std::cout << "----- update(): result=" << result << std::endl;
        std::cout << "----- update(): result->ColumnHeaders[0]=" << result->ColumnHeaders[0] << std::endl;


        // debug - does it generate expected data?
        int index = 0;
        // Print out column headers... typically time and species.
        for (int col = 0; col < result->CSize; col++)
        {
            // std::cout << result->ColumnHeaders[index++];
            // std::cout << std::left << std::setw(15) << result->ColumnHeaders[index++];
            std::cout << std::left << result->ColumnHeaders[index++];
            // if (col < result->CSize - 1)
            // {
            // 	// std::cout << "\t";
            // 	std::cout << "  ";
            // }
        }
        std::cout << "\n";

        return 43;
	}
	
	bool need_update() {
		return PhysiCell::PhysiCell_globals.current_time >= this->next_librr_run;
	}
	
	double get_parameter_value(std::string name) {
		// return this->maboss.get_parameter_value(name);
		// return this->rrHandle.get_parameter_value(name);
        return 42.0;
	}
	
	void set_parameter_value(std::string name, double value) {
		// this->maboss.set_parameter_value(name, value);
        return;
	}
	
	std::string get_state() {
		// return this->maboss.get_state();
        // const std::string s0 ( "bogus get_state" );
        // return s0;
        return this->sbml_filename;
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