#include "shape_from_model.hpp"
#include "vertex_array.hpp"
#include "index_array.hpp"
#include "shape.hpp"
#include "../../stdlib/map.hpp"
#include <sge/model/object.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/io/cout.hpp>

insula::physics::triangle_mesh::shape_auto_ptr
insula::physics::triangle_mesh::shape_from_model(
	sge::model::object_ptr const m)
{
	fcppt::string const part = 
		m->part_names().front();

	sge::model::index_sequence const index_preimage = 
		m->indices(
			part);

	// The normals point in the wrong direction if we just copy our
	// index array. So we swap the index order
	index_array indices;

	indices.reserve(
		static_cast<index_array::size_type>(
			index_preimage.size()));

	for(
		index_array::size_type i = 0; 
		i < static_cast<index_array::size_type>(index_preimage.size()); 
		i = static_cast<index_array::size_type>(i+3))
	{
		indices.push_back(
			static_cast<index_array::value_type>(
				index_preimage[i]));	
		indices.push_back(
			static_cast<index_array::value_type>(
				index_preimage[i+2]));	
		indices.push_back(
			static_cast<index_array::value_type>(
				index_preimage[i+1]));	
	}

	return 
		shape_auto_ptr(
			new shape(
				stdlib::map<vertex_array>(
					m->vertices(part),
					[](sge::model::vertex_sequence::const_reference v) 
					{ 
						// NOTE: vertex_array is/was btVector3, hence no
						// structure_cast here
						return 
							vertex_array::value_type(
								static_cast<scalar>(
									v[0]),
								static_cast<scalar>(
									v[1]),
								static_cast<scalar>(
									v[2]));
					}),
				indices));
}
