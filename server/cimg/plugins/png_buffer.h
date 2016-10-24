#ifndef cimg_plugin_png_buffer
#define cimg_plugin_png_buffer

struct buffer_png_reader
{
   const char* const buffer;
   int32_t total;
   int32_t readed;
   buffer_png_reader(const char* const b, const unsigned t)
       :buffer(b)
       ,total(t)
       ,readed(0)
   {
   }
};
static void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
   png_size_t byteCountToRead)
 {
     png_voidp io_ptr = png_get_io_ptr(png_ptr);
     if(io_ptr == NULL)
        return;   // add custom error handling here

     // using pulsar::InputStream
     // -> replace with your own data source interface
     buffer_png_reader& inputStream = *(buffer_png_reader*)io_ptr;
     const size_t bytesRead = std::min<size_t>(inputStream.total - inputStream.readed, byteCountToRead);
     memcpy(outBytes, inputStream.buffer + inputStream.readed, bytesRead);
    inputStream.readed += bytesRead;
    //  if((png_size_t)bytesRead != byteCount)
    //     return;   // add custom error handling here
 }

CImg& load_png_buffer(const char *const buffer, const unsigned buffer_size)
{

    unsigned char pngCheck[8] = { 0 };
    memcpy(pngCheck, buffer, 8);
    if (png_sig_cmp(pngCheck,0,8)) {

      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Invalid PNG format.",
                            cimg_instance);
    }

    // Setup PNG structures for read
    png_voidp user_error_ptr = 0;
    png_error_ptr user_error_fn = 0, user_warning_fn = 0;
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,user_error_ptr,user_error_fn,user_warning_fn);
    if (!png_ptr) {
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Failed to initialize 'png_ptr' structure.",
                            cimg_instance);
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_read_struct(&png_ptr,(png_infopp)0,(png_infopp)0);
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Failed to initialize 'info_ptr' structure.",
                            cimg_instance);
    }
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Failed to initialize 'end_info' structure.",
                            cimg_instance);
    }

    buffer_png_reader reader(buffer + 8, buffer_size - 8);
    png_set_read_fn(png_ptr, &reader, ReadDataFromInputStream);
    // Error handling callback for png file reading
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_read_struct(&png_ptr, &end_info, (png_infopp)0);
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Encountered unknown fatal error in libpng.",
                            cimg_instance);
    }
    // png_init_io(png_ptr, nfile);
    png_set_sig_bytes(png_ptr, 8);

    // Get PNG Header Info up to data block
    png_read_info(png_ptr,info_ptr);
    png_uint_32 W, H;
    int bit_depth, color_type, interlace_type;
    bool is_gray = false;
    png_get_IHDR(png_ptr,info_ptr,&W,&H,&bit_depth,&color_type,&interlace_type,(int*)0,(int*)0);
    // if (bits_per_pixel) *bits_per_pixel = (unsigned int)bit_depth;

    // Transforms to unify image data
    if (color_type==PNG_COLOR_TYPE_PALETTE) {
      png_set_palette_to_rgb(png_ptr);
      color_type = PNG_COLOR_TYPE_RGB;
      bit_depth = 8;
    }
    if (color_type==PNG_COLOR_TYPE_GRAY && bit_depth<8) {
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      is_gray = true;
      bit_depth = 8;
    }
    if (png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS)) {
      png_set_tRNS_to_alpha(png_ptr);
      color_type |= PNG_COLOR_MASK_ALPHA;
    }
    if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA) {
      png_set_gray_to_rgb(png_ptr);
      color_type |= PNG_COLOR_MASK_COLOR;
      is_gray = true;
    }
    if (color_type==PNG_COLOR_TYPE_RGB)
      png_set_filler(png_ptr,0xffffU,PNG_FILLER_AFTER);

    png_read_update_info(png_ptr,info_ptr);
    if (bit_depth!=8 && bit_depth!=16) {
      png_destroy_read_struct(&png_ptr,&end_info,(png_infopp)0);
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Invalid bit depth %u in file '%s'.",
                            cimg_instance);
    }
    const int byte_depth = bit_depth>>3;

    // Allocate Memory for Image Read
    png_bytep *const imgData = new png_bytep[H];
    for (unsigned int row = 0; row<H; ++row) imgData[row] = new png_byte[(size_t)byte_depth*4*W];
    png_read_image(png_ptr,imgData);
    png_read_end(png_ptr,end_info);

    // Read pixel data
    if (color_type!=PNG_COLOR_TYPE_RGB && color_type!=PNG_COLOR_TYPE_RGB_ALPHA) {
      png_destroy_read_struct(&png_ptr,&end_info,(png_infopp)0);
      throw CImgIOException(_cimg_instance
                            "load_jpeg_buffer(): Invalid color coding type %u in file '%s'.",
                            cimg_instance);
    }
    const bool is_alpha = (color_type==PNG_COLOR_TYPE_RGBA);
    try { assign(W,H,1,(is_gray?1:3) + (is_alpha?1:0)); }
    catch (...) { throw; }
    T
      *ptr_r = data(0,0,0,0),
      *ptr_g = is_gray?0:data(0,0,0,1),
      *ptr_b = is_gray?0:data(0,0,0,2),
      *ptr_a = !is_alpha?0:data(0,0,0,is_gray?1:3);
    switch (bit_depth) {
    case 8 : {
      cimg_forY(*this,y) {
        const unsigned char *ptrs = (unsigned char*)imgData[y];
        cimg_forX(*this,x) {
          *(ptr_r++) = (T)*(ptrs++);
          if (ptr_g) *(ptr_g++) = (T)*(ptrs++); else ++ptrs;
          if (ptr_b) *(ptr_b++) = (T)*(ptrs++); else ++ptrs;
          if (ptr_a) *(ptr_a++) = (T)*(ptrs++); else ++ptrs;
        }
      }
    } break;
    case 16 : {
      cimg_forY(*this,y) {
        const unsigned short *ptrs = (unsigned short*)(imgData[y]);
        if (!cimg::endianness()) cimg::invert_endianness(ptrs,4*_width);
        cimg_forX(*this,x) {
          *(ptr_r++) = (T)*(ptrs++);
          if (ptr_g) *(ptr_g++) = (T)*(ptrs++); else ++ptrs;
          if (ptr_b) *(ptr_b++) = (T)*(ptrs++); else ++ptrs;
          if (ptr_a) *(ptr_a++) = (T)*(ptrs++); else ++ptrs;
        }
      }
    } break;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // Deallocate Image Read Memory
    cimg_forY(*this,n) delete[] imgData[n];
    delete[] imgData;
    return *this;
}

#endif
