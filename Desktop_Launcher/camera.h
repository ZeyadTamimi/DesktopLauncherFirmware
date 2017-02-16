#ifndef CAMERA_H_
#define CAMERA_H_

//===================================================================
// Includes
//===================================================================
#include <stdint.h>


#define CAMERA_FRAME_PADDING 5

//===================================================================
// Public Types
//===================================================================
/*!
 * \brief The resolution enum holds all image resolutions supported by the
 *        camera.
 */
typedef enum resolution
{
    FRAME_640x480 = 0x00,
    FRAME_320x240 = 0x11,
    FRAME_160x120 = 0x22,
    UNDEFINED = 0xFF
}resolution;

//===================================================================
// Public Function Declaration
//===================================================================
/*!
 * \brief This function initializes the serial connection to the camera and
 *        clears the internal receive buffers.
 */
void cam_init(void);

/*!
 * \brief Queries the camera for its current resolution.
 * \return This function returns the current set resolution for the camera as
 *         a resolution enum value.
 */
resolution get_image_resolution();

/*!
 * \brief Sets the camera's resolution for all new photos.
 * \return This function returns the current set resolution for the camera as
 *         a resolution enum value.
 * \param [in] new_resolution, The new image resolusion to request from the
 *             camera.
 */
int set_image_resolution(resolution new_resolution);

int take_picture(void);

uint32_t frame_length(void);

int camera_reset(void);

uint8_t *read_picture(uint8_t n);
int resume_picture(void);
read_picture_to_ptr(uint8_t * jpeg_buffer, uint8_t n);

int set_motion_detect(int flag);
int get_motion_detect(void);
int motion_detected(unsigned long timeout);

#endif /* CAMERA_H_ */
