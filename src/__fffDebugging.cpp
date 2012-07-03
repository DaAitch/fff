
#include "../include/__fffDebugging.h"

void _fff_NAMESPACE_PREFIX __rtclcCheckErr(
	Int err,
	const Char *subject)
{
	if(err!=CL_SUCCESS)
	{
		StringStream filename;
		filename << "__rtclcCheckErrFailure_";
		filename << time(NULL);
		filename << ".log";

		{
			std::fstream logFile(filename.str().c_str(), std::ios::out);

			LogStream logger(logFile);

			logger("OpenCL Error");
			!(logger >> (rtclcDebugErrorByName(err)) << subject);
			logger();
			logFile.close();
		}


		std::cout << "Something went wrong, see: " << filename.str() << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}



String _fff_NAMESPACE_PREFIX rtclcDebugErrorByName(
	Int errorNum)
{
	switch(errorNum)
	{
		_fff_CASE_ERR( CL_DEVICE_NOT_FOUND                         )
		_fff_CASE_ERR( CL_DEVICE_NOT_AVAILABLE                     )
		_fff_CASE_ERR( CL_COMPILER_NOT_AVAILABLE                   )
		_fff_CASE_ERR( CL_MEM_OBJECT_ALLOCATION_FAILURE            )
		_fff_CASE_ERR( CL_OUT_OF_RESOURCES                         )
		_fff_CASE_ERR( CL_OUT_OF_HOST_MEMORY                       )
		_fff_CASE_ERR( CL_PROFILING_INFO_NOT_AVAILABLE             )
		_fff_CASE_ERR( CL_MEM_COPY_OVERLAP                         )
		_fff_CASE_ERR( CL_IMAGE_FORMAT_MISMATCH                    )
		_fff_CASE_ERR( CL_IMAGE_FORMAT_NOT_SUPPORTED               )
		_fff_CASE_ERR( CL_BUILD_PROGRAM_FAILURE                    )
		_fff_CASE_ERR( CL_MAP_FAILURE                              )
		_fff_CASE_ERR( CL_MISALIGNED_SUB_BUFFER_OFFSET             )
		_fff_CASE_ERR( CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST )

		_fff_CASE_ERR( CL_INVALID_VALUE                            )
		_fff_CASE_ERR( CL_INVALID_DEVICE_TYPE                      )
		_fff_CASE_ERR( CL_INVALID_PLATFORM                         )
		_fff_CASE_ERR( CL_INVALID_DEVICE                           )
		_fff_CASE_ERR( CL_INVALID_CONTEXT                          )
		_fff_CASE_ERR( CL_INVALID_QUEUE_PROPERTIES                 )
		_fff_CASE_ERR( CL_INVALID_COMMAND_QUEUE                    )
		_fff_CASE_ERR( CL_INVALID_HOST_PTR                         )
		_fff_CASE_ERR( CL_INVALID_MEM_OBJECT                       )
		_fff_CASE_ERR( CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          )
		_fff_CASE_ERR( CL_INVALID_IMAGE_SIZE                       )
		_fff_CASE_ERR( CL_INVALID_SAMPLER                          )
		_fff_CASE_ERR( CL_INVALID_BINARY                           )
		_fff_CASE_ERR( CL_INVALID_BUILD_OPTIONS                    )
		_fff_CASE_ERR( CL_INVALID_PROGRAM                          )
		_fff_CASE_ERR( CL_INVALID_PROGRAM_EXECUTABLE               )
		_fff_CASE_ERR( CL_INVALID_KERNEL_NAME                      )
		_fff_CASE_ERR( CL_INVALID_KERNEL_DEFINITION                )
		_fff_CASE_ERR( CL_INVALID_KERNEL                           )
		_fff_CASE_ERR( CL_INVALID_ARG_INDEX                        )
		_fff_CASE_ERR( CL_INVALID_ARG_VALUE                        )
		_fff_CASE_ERR( CL_INVALID_ARG_SIZE                         )
		_fff_CASE_ERR( CL_INVALID_KERNEL_ARGS                      )
		_fff_CASE_ERR( CL_INVALID_WORK_DIMENSION                   )
		_fff_CASE_ERR( CL_INVALID_WORK_GROUP_SIZE                  )
		_fff_CASE_ERR( CL_INVALID_WORK_ITEM_SIZE                   )
		_fff_CASE_ERR( CL_INVALID_GLOBAL_OFFSET                    )
		_fff_CASE_ERR( CL_INVALID_EVENT_WAIT_LIST                  )
		_fff_CASE_ERR( CL_INVALID_EVENT                            )
		_fff_CASE_ERR( CL_INVALID_OPERATION                        )
		_fff_CASE_ERR( CL_INVALID_GL_OBJECT                        )
		_fff_CASE_ERR( CL_INVALID_BUFFER_SIZE                      )
		_fff_CASE_ERR( CL_INVALID_MIP_LEVEL                        )
		_fff_CASE_ERR( CL_INVALID_GLOBAL_WORK_SIZE                 )
		_fff_CASE_ERR( CL_INVALID_PROPERTY                         )
	default:
		return String("Unknown Error");
	}
}