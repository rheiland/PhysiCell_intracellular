#include "librr_intracellular.h"

RoadRunnerIntracellular::RoadRunnerIntracellular() : Intracellular()
{
	type = "sbml";
    std::cout << "====== " << __FUNCTION__ << "() type=" << type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "() sbml_file = " <<  sbml_file << std::endl;
	initial_values.clear();
	// mutations.clear();
	parameters.clear();
}

// constructor using XML node
RoadRunnerIntracellular::RoadRunnerIntracellular(pugi::xml_node& node)
{
	type = "roadrunner";
	initialize_intracellular_from_pugixml(node);
    std::cout << "====== " << __FUNCTION__ << "(node) type=" << type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "(node) sbml_file = " <<  sbml_file << std::endl;
    std::cout << "====== " << __FUNCTION__ << "(node) this=" <<  this << std::endl;
}

RoadRunnerIntracellular::RoadRunnerIntracellular(RoadRunnerIntracellular* copy) 
{
	type = copy->type;
	// cfg_filename = copy->cfg_filename;
	time_step = copy->time_step;
	discrete_time = copy->discrete_time;
	time_tick = copy->time_tick;
	scaling = copy->scaling;
	initial_values = copy->initial_values;
	// mutations = copy->mutations;
	parameters = copy->parameters;
	
}

void RoadRunnerIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        this->sbml_file = PhysiCell::xml_get_my_string_value (node_sbml); 
        // sbml_file = PhysiCell::xml_get_my_string_value (node_sbml); 
        // std::cout << "\n========== " << __FILE__ << ", " << __FUNCTION__ << ": ------- sbml_file = " << sbml_file << std::endl;
        // std::cout << "\n------------- "  << __FUNCTION__ << ": sbml_file = " << sbml_file << std::endl;
        std::cout << "\n------------- "  << __FUNCTION__ << ": this->sbml_file = " << this->sbml_file << std::endl;
    }
	
	pugi::xml_node node_parameters = node.child( "parameters" );
	if( node_parameters )
	{
		pugi::xml_node node_parameter = node_parameters.child( "parameter" );
		while( node_parameter )
		{
			std::string param_name = node_parameter.attribute( "name" ).value(); 
			double param_value = PhysiCell::xml_get_my_double_value( node_parameter );
			
			parameters[param_name] = param_value;
			
			node_parameter = node_parameter.next_sibling( "parameter" ); 
		}
	}
	
	// maboss.init_maboss(bnd_filename, cfg_filename);
	// maboss.mutate(mutations);
	// maboss.set_initial_values(initial_values);
	// maboss.set_parameters(parameters);	
	
	// pugi::xml_node node_timestep = node.child( "time_step" ); 
	// if( node_timestep )
	// { 
	// 	time_step = PhysiCell::xml_get_my_double_value( node_timestep );
	// 	maboss.set_update_time_step(time_step);
	// }
}

int RoadRunnerIntracellular::start()
{
    std::cout << "------ librr_intracellular.h: start() called\n";
    // this->maboss.restart_node_values();
    this->rrHandle = createRRInstance();

    // if (!rrc::loadSBML (rrHandle, get_cell_definition("lung epithelium").sbml_filename.c_str())) 
    std::cout << "------ librr_intracellular.h:start(): sbml_file = " << sbml_file << std::endl;
    // if ( !rrc::loadSBML(rrHandle, (this->sbml_file).c_str() ) )
    if (!rrc::loadSBML(rrHandle, "./config/Toy_SBML_Model_1.xml") )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        return -1;
        // 	printf ("Error message: %s\n", getLastError());
        // 	getchar ();
        // 	exit (0);
    }

    std::cout << "----- start(): rrHandle=" << rrHandle << std::endl;
    return 0;
}

int RoadRunnerIntracellular::update()
{
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

        index = 0;
        // Print out the data
        for (int row = 0; row < result->RSize; row++)
        {
            for (int col = 0; col < result->CSize; col++)
            {
                // std::cout << result->Data[index++];
                std::cout << std::left << std::setw(15) << result->Data[index++];
                // if (col < result->CSize -1)
                // {
                // 	// std::cout << "\t";
                // 	std::cout << "  ";
                // }
            }
            std::cout << "\n";
        }
        // int idx = (result->RSize - 1) * result->CSize + 1;
        // std::cout << "Saving last energy value (cell custom var) = " << result->Data[idx] << std::endl;
        // pCell->custom_data[energy_cell_idx]  = result->Data[idx];

        return 0;
}

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<RoadRunnerIntracellular*>(phenotype.intracellular);
}

// void RoadRunnerIntracellular::save_PhysiBoSS(std::string path, std::string index)
// {

// 	std::string state_file_name = path + "/states_" + index + ".csv";
					
// 	std::ofstream state_file( state_file_name );
	
// 	state_file << "ID,state" << std::endl;

// 	for( auto cell : *PhysiCell::all_cells )
// 		state_file << cell->ID << "," << cell->phenotype.intracellular->get_state() << std::endl;
		
// 	state_file.close();

// }