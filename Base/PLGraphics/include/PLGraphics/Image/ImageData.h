/*********************************************************\
 *  File: ImageData.h                                    *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLGRAPHICS_IMAGEDATA_H__
#define __PLGRAPHICS_IMAGEDATA_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/Base/RefCount.h>
#include <PLMath/Vector3i.h>
#include "PLGraphics/PLGraphics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ImagePalette;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image data, contains the actual data of an image buffer
*
*  @note
*    - Implementation of the proxy design pattern, this class is the real subject
*/
class ImageData : protected PLGeneral::RefCount<ImageData> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ImageBuffer;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		ImageData();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ImageData(const ImageData &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ImageData();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this object
		*/
		ImageData &operator =(const ImageData &cSource);

		/**
		*  @brief
		*    Create image
		*
		*  @param[in] nDataFormat
		*    Desired data format
		*  @param[in] nColorFormat
		*    Desired color format
		*  @param[in] vSize
		*    Image size
		*  @param[in] nCompression
		*    Compression type
		*
		*  @remarks
		*    Please note that a call to Create() will not immediatly create the image buffer itself.
		*    The image buffer is created on the first call to GetData() or GetCompressedData() respectively.
		*/
		void CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const PLMath::Vector3i &vSize, ECompression nCompression = CompressionNone);

		/**
		*  @brief
		*    Clear data
		*/
		void Clear();

		/**
		*  @brief
		*    Create a test image
		*
		*  @param[in] nTestImage
		*    ID of test image
		*
		*  @remarks
		*    Only 2D and 3D test images are allowed here. For images containing more than
		*    one image buffer, you have to use Image::CreateTestImage().
		*/
		void CreateTestImage(ETestImage nTestImage = TestImage2DSimple);

		/**
		*  @brief
		*    Get data format
		*
		*  @return
		*    Data format
		*/
		EDataFormat GetDataFormat() const;

		/**
		*  @brief
		*    Get color format
		*
		*  @return
		*    Color format
		*/
		EColorFormat GetColorFormat() const;

		/**
		*  @brief
		*    Get compression type
		*
		*  @return
		*    Compression type
		*/
		ECompression GetCompression() const;

		/**
		*  @brief
		*    Set compression type
		*
		*  @param[in] nCompression
		*    Compression type
		*
		*  @remarks
		*    This will invalidate the compressed image data buffer
		*/
		void SetCompression(ECompression nCompression);

		/**
		*  @brief
		*    Get image size
		*
		*  @return
		*    Size of image
		*/
		PLMath::Vector3i GetSize() const;

		/**
		*  @brief
		*    Check if uncompressed data is available
		*
		*  @return
		*    'true' if image data is available, else 'false'
		*/
		bool HasData() const;

		/**
		*  @brief
		*    Get size of image data
		*
		*  @return
		*    Size of image data in bytes
		*/
		PLGeneral::uint32 GetDataSize() const;

		/**
		*  @brief
		*    Get image data
		*
		*  @return
		*    Pointer to image data, a null pointer on error
		*
		*  @remarks
		*    If the image contains no uncompressed data, but compressed image data is available the data,
		*    will be decompressed automatically and stored in the decompressed image buffer this method returns.
		*/
		const PLGeneral::uint8 *GetData() const;
		PLGeneral::uint8 *GetData();

		/**
		*  @brief
		*    Check if compressed data is available
		*
		*  @return
		*    'true' if compressed image data is available, else 'false'
		*/
		bool HasCompressedData() const;

		/**
		*  @brief
		*    Get size of compressed image data
		*
		*  @return
		*    Size of compressed image data in bytes
		*/
		PLGeneral::uint32 GetCompressedDataSize() const;

		/**
		*  @brief
		*    Get compressed image data
		*
		*  @return
		*    Pointer to compressed image data, a null pointer on error
		*/
		const PLGeneral::uint8 *GetCompressedData() const;
		PLGeneral::uint8 *GetCompressedData();

		/**
		*  @brief
		*    Compress image data
		*
		*  @remarks
		*    If the image contains uncompressed image data, the data will be
		*    compressed and stored in the compressed image buffer.
		*/
		bool Compress();

		/**
		*  @brief
		*    Decompress image data
		*
		*  @remarks
		*    If the image contains compressed image data, the data will be
		*    decompressed and stored in the decompressed image buffer.
		*/
		bool Decompress();

		/**
		*  @brief
		*    Set color palette
		*
		*  @param[in] pPalette
		*    Pointer to color palette, or a null pointer if the image has no palette
		*/
		void SetPalette(ImagePalette *pPalette);

		/**
		*  @brief
		*    Get color palette
		*
		*  @return
		*    Pointer to color palette, or a null pointer if the image has no palette
		*/
		const ImagePalette *GetPalette() const;
		ImagePalette *GetPalette();

		/**
		*  @brief
		*    Copy provided image data into this image data
		*
		*  @param[in] pnData
		*    Data to copy into this image data, must have enough bytes to fill the whole image! The given pointer must be valid!
		*/
		void CopyData(const PLGeneral::uint8 *pnData);

		/**
		*  @brief
		*    Let this image data takeover provided image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be taken over by this image data, must have enough bytes to fill the whole image data! The given pointer must be valid!
		*/
		void TakeoverData(PLGeneral::uint8 *pnData);

		/**
		*  @brief
		*    Let this image data share provided image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be shared by this image data, must have enough bytes to fill the whole image data! The given pointer must be valid!
		*/
		void ShareData(PLGeneral::uint8 *pnData);

		/**
		*  @brief
		*    Create image buffer
		*
		*  @remarks
		*    The buffer is only created, if it doesn't exist before. So it's safe to call it
		*    in order to get a valid buffer before accessing it, this won't destroy anything.
		*/
		void CreateBuffer();

		/**
		*  @brief
		*    Create compressed image buffer
		*
		*  @remarks
		*    The buffer is only created, if it doesn't exist before. So it's safe to call it
		*    in order to get a valid buffer before accessing it, this won't destroy anything.
		*/
		void CreateCompressedBuffer();

		/**
		*  @brief
		*    Destroy image buffer
		*/
		void DestroyBuffer();

		/**
		*  @brief
		*    Destroy compressed image buffer
		*/
		void DestroyCompressedBuffer();

		/**
		*  @brief
		*    Calculates the size of the compressed image buffer
		*/
		void CalculateCompressedImageBufferSize();

		/**
		*  @brief
		*    Decodes a DXT color block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*  @param[in]  nCompression
		*    Used compression
		*  @param[in]  nRedIndex
		*    Index of the red color component
		*  @param[in]  nBlueIndex
		*    Index of the blue color component
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXTColorBlock(PLGeneral::uint8 *pnDestination, const PLGeneral::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset, ECompression nCompression, int nRedIndex, int nBlueIndex) const;

		/**
		*  @brief
		*    Decodes a DXT3 alpha block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXT3AlphaBlock(PLGeneral::uint8 *pnDestination, const PLGeneral::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const;

		/**
		*  @brief
		*    Decodes a DXT5 alpha block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXT5AlphaBlock(PLGeneral::uint8 *pnDestination, const PLGeneral::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EDataFormat			 m_nDataFormat;			/**< Data format of the image */
		EColorFormat		 m_nColorFormat;		/**< Color format of the image */
		ECompression		 m_nCompression;		/**< Compression format */
		PLMath::Vector3i	 m_vSize;				/**< Size of the image */
		PLGeneral::uint8	*m_pData;				/**< Uncompressed image data, can be a null pointer */
		PLGeneral::uint32	 m_nDataSize;			/**< Size of uncompressed image data in bytes */
		bool				 m_bDataShared;			/**< If 'true', the uncompressed image data is just shared and this image data instance is not allowed to deleted it (the user keeps the image data ownership) */
		PLGeneral::uint8	*m_pCompressedData;		/**< Compressed image data, can be a null pointer */
		PLGeneral::uint32	 m_nCompressedSize;		/**< Size of compressed image data in bytes */
		ImagePalette		*m_pPalette;			/**< Color palette, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGEDATA_H__