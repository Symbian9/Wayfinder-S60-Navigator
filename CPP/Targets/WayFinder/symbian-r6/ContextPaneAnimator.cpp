/*
  Copyright (c) 1999 - 2010, Vodafone Group Services Ltd
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  * Neither the name of the Vodafone Group Services Ltd nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <e32base.h>
#include <fbs.h>
#ifdef NAV2_CLIENT_SERIES60_V3
#include <aknutils.h>
#endif
#include "AnimatorFrame.h"
#include "ContextPaneAnimator.h"
#include <bitstd.h>
#include <bitdev.h>
/* For the bitmap enum (autogenerated for the MBM file) */
#include "wficons.mbg"
#include "WFTextUtil.h"
#include "WFLayoutUtils.h"

#define LARGE_GPS_WIDTH            32
#define GPS_WIDTH                  32
#define ANIMATION_FRAME_SIZE       TSize(32, 30)
#define GPS_FRAME_SIZE             TSize(32, 12)
#define MERGE_TO_CONTEXT_PANE_SIZE TSize(32, 42)
#define GPS_ICON_POS               TPoint(0, 30)


// Since the height is different for the icons,
// and since we calculate the size of the icon
// on s60v3 based on the size of the icon on 
// s60v2 we need to keep track of the height
// on the icons. The width is allways 32 pixels.
const TInt GpsImageHeight[] = {
   12,
   32,
   32,
   32,
   32,
   32,
   12,
   12,
   12,
   12,
};

CContextPaneAnimator::CContextPaneAnimator(const TDesC &mbmPath) :
   iCurrentOk(EFalse),
   iCurrentFrameIndex(0),
   iCurrentGpsStatus(0),
   iCurrentFrame(NULL),
   numAnimationFrames(BUSY_ICON_NUM_FRAMES),
   iAnimationFrames(NULL),
   numGpsQualityFrames(10),
   iGpsQualityFrames(NULL)
{
   WFTextUtil::TDesCopy(iMbmPath, mbmPath);
}

CContextPaneAnimator::~CContextPaneAnimator()
{
   if (iAnimationFrames) {
      for (TInt i = 0; i < numAnimationFrames ; i++ ) {
         if (iAnimationFrames[i]) {
            delete iAnimationFrames[i];
            iAnimationFrames[i]=NULL;
         }
      }
      delete[] iAnimationFrames;
   }

   if (iGpsQualityFrames) {
      for (TInt i = 0; i < numGpsQualityFrames ; i++ ) {
         if (iGpsQualityFrames[i]) {
            delete iGpsQualityFrames[i];
            iGpsQualityFrames[i]=NULL;
         }
      }
      delete[] iGpsQualityFrames;
   }
   delete iCurrentFrame;
   iCurrentFrame = NULL;
}

TBool
CContextPaneAnimator::CurrentOk()
{
   return iCurrentOk;
}

CAnimatorFrame *
CContextPaneAnimator::GetAnimationFrameL()
{
   return GetAnimationFrameL(iCurrentFrameIndex);
}

CAnimatorFrame *
CContextPaneAnimator::GetAnimationFrameL(TInt index)
{
   TInt AnimToMbmId[] = {
      BUSY_ICON_LIST,
   };

   if (!iAnimationFrames) {
      /* Need to allocate the array. */
      iAnimationFrames = new (ELeave) CAnimatorFrame*[numAnimationFrames];
      for (int i = 0; i < numAnimationFrames; i++) {
         iAnimationFrames[i] = NULL;
      }
   }
   if (!iAnimationFrames[index]) {
      /* Need to load bitmaps for this frame. */
      CAnimatorFrame *frame = new (ELeave) CAnimatorFrame();
      CleanupStack::PushL(frame);
      frame->LoadBitmapAndMaskL(iMbmPath, 
                                AnimToMbmId[index], 
                                AnimToMbmId[index+numAnimationFrames],
                                WFLayoutUtils::CalculateSizeUsingContextPane(ANIMATION_FRAME_SIZE));
      iAnimationFrames[index] = frame;
      CleanupStack::Pop(frame);
   }

   return iAnimationFrames[index];
}

#ifdef NAV2_CLIENT_SERIES60_V3

CAnimatorFrame*
CContextPaneAnimator::GetSvgAnimationFrameL()
{
   return GetSvgAnimationFrameL(iCurrentFrameIndex);
}

CAnimatorFrame *
CContextPaneAnimator::GetSvgAnimationFrameL(TInt index)
{
   TInt AnimToMbmId[] = {
      EMbmWficonsBusy1,
      EMbmWficonsBusy2,
      EMbmWficonsBusy3,
      EMbmWficonsBusy4,
      EMbmWficonsBusy5,
      EMbmWficonsBusy6,
      EMbmWficonsBusy7,
      EMbmWficonsBusy8,
      EMbmWficonsBusy1_mask,
      EMbmWficonsBusy2_mask,
      EMbmWficonsBusy3_mask,
      EMbmWficonsBusy4_mask,
      EMbmWficonsBusy5_mask,
      EMbmWficonsBusy6_mask,
      EMbmWficonsBusy7_mask,
      EMbmWficonsBusy8_mask,
   };
   if (!iSvgAnimationFrames) {
      /* Need to allocate the array. */
      iSvgAnimationFrames = new (ELeave) CAnimatorFrame*[numAnimationFrames];
      for (int i = 0; i < numAnimationFrames; i++) {
         iSvgAnimationFrames[i] = NULL;
      }
   }
   if (!iSvgAnimationFrames[index]) {
      /* Need to load bitmaps for this frame. */
      CAnimatorFrame *frame = new (ELeave) CAnimatorFrame();
      CleanupStack::PushL(frame);
      frame->LoadBitmapAndMaskL(iMbmPath, 
                                AnimToMbmId[index], 
                                AnimToMbmId[index+numAnimationFrames],
                                WFLayoutUtils::CalculateSizeUsingFullScreen(ANIMATION_FRAME_SIZE));
      iSvgAnimationFrames[index] = frame;
      CleanupStack::Pop(frame);
   }

   return iSvgAnimationFrames[index];
}
#endif

CAnimatorFrame *
CContextPaneAnimator::GetLargeGpsFrameL()
{
   return GetLargeGpsFrameL(iCurrentGpsStatus);
}

CAnimatorFrame *
CContextPaneAnimator::GetGpsFrameL()
{
   return GetGpsFrameL(iCurrentGpsStatus);
}

CAnimatorFrame *
CContextPaneAnimator::GetLargeGpsFrameL(TInt index)
{
#ifdef NAV2_CLIENT_SERIES60_V3
   TInt LargeGpsToMbmId[] = {
      EMbmWficonsGps_blank,
      EMbmWficonsGps_icon_5,
      EMbmWficonsGps_icon_4,
      EMbmWficonsGps_icon_3,
      EMbmWficonsGps_icon_2,
      EMbmWficonsGps_icon_1,
      EMbmWficonsDemo1_speed_h,
      EMbmWficonsDemo1_speed_1,
      EMbmWficonsDemo1_speed_2,
      EMbmWficonsDemo1_speed_4,
      EMbmWficonsGps_blank_mask,
      EMbmWficonsGps_icon_5_mask,
      EMbmWficonsGps_icon_4_mask,
      EMbmWficonsGps_icon_3_mask,
      EMbmWficonsGps_icon_2_mask,
      EMbmWficonsGps_icon_1_mask,
      EMbmWficonsDemo1_speed_h_mask,
      EMbmWficonsDemo1_speed_1_mask,
      EMbmWficonsDemo1_speed_2_mask,
      EMbmWficonsDemo1_speed_4_mask,
   };   
#else
   TInt LargeGpsToMbmId[] = {
      EMbmWficonsGps1,
      EMbmWficons32x32_gps_none,
      EMbmWficons32x32_gps_0,
      EMbmWficons32x32_gps_1,
      EMbmWficons32x32_gps_2,
      EMbmWficons32x32_gps_3,
      EMbmWficonsDemo1_speed_h,
      EMbmWficonsDemo1_speed_1,
      EMbmWficonsDemo1_speed_2,
      EMbmWficonsDemo1_speed_4,
      EMbmWficonsGps1_m_no_gps,
      EMbmWficons32x32_gps_none_m,
      EMbmWficons32x32_gps_0_m,
      EMbmWficons32x32_gps_1_m,
      EMbmWficons32x32_gps_2_m,
      EMbmWficons32x32_gps_3_m,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
   };
#endif

   if (!iLargeGpsQualityFrames) {
      /* Need to create array. */
      iLargeGpsQualityFrames = new CAnimatorFrame*[numGpsQualityFrames];
      for (int i = 0; i < numGpsQualityFrames; i++) {
         iLargeGpsQualityFrames[i] = NULL;
      }
   }
   if (!iLargeGpsQualityFrames[index]) {
      /* Need to load bitmaps for this frame. */
      CAnimatorFrame *frame = new (ELeave) CAnimatorFrame();
      CleanupStack::PushL(frame);
      frame->LoadBitmapAndMaskL(iMbmPath, 
                                LargeGpsToMbmId[index], 
                                LargeGpsToMbmId[index+numGpsQualityFrames], 
                                WFLayoutUtils::
                                CalculateSizeUsingFullScreen(LARGE_GPS_WIDTH,
                                                             GpsImageHeight[index]), 
                                EAspectRatioPreservedAndUnusedSpaceRemoved);      

      iLargeGpsQualityFrames[index] = frame;
      CleanupStack::Pop(frame);
   }
   return iLargeGpsQualityFrames[index];
}

#ifdef NAV2_CLIENT_SERIES60_V3
void
CContextPaneAnimator::RecalcLargeGpsFrameSize()
{
   if (iLargeGpsQualityFrames) {
      for (int i = 0; i < numGpsQualityFrames; i++) {
         if (iLargeGpsQualityFrames[i]) {
            iLargeGpsQualityFrames[i]->
               SetBitmapSize(WFLayoutUtils::CalculateSizeUsingFullScreen(LARGE_GPS_WIDTH,
                                                                         GpsImageHeight[i]), 
                             EAspectRatioPreservedAndUnusedSpaceRemoved);
         }
      }
   }
}

void
CContextPaneAnimator::RecalcContextAnimSizes()
{
   if (iSvgAnimationFrames) {
      for (int i = 0; i < numAnimationFrames; i++) {
         if (iAnimationFrames[i]) { 
            iAnimationFrames[i]->
               SetBitmapSize(WFLayoutUtils::
                             CalculateSizeUsingContextPane(ANIMATION_FRAME_SIZE));
         }
      }
   }
   if (iGpsQualityFrames) {
      for (int i = 0; i < numGpsQualityFrames; i++) {
         if (iGpsQualityFrames[i]) { 
            iGpsQualityFrames[i]->
               SetBitmapSize(WFLayoutUtils::
                             CalculateSizeUsingContextPane(GPS_FRAME_SIZE));
         }
      }      
   }
}

#endif

CAnimatorFrame *
CContextPaneAnimator::GetGpsFrameL(TInt index)
{
#ifdef NAV2_CLIENT_SERIES60_V3
   TInt GpsToMbmId[] = {
      EMbmWficonsGps_blank,
      EMbmWficonsGps_no_signal,
      EMbmWficonsGps_useless,
      EMbmWficonsGps_poor,
      EMbmWficonsGps_descent,
      EMbmWficonsGps_good,
      EMbmWficonsDemo1_speed_h,
      EMbmWficonsDemo1_speed_1,
      EMbmWficonsDemo1_speed_2,
      EMbmWficonsDemo1_speed_4,
      EMbmWficonsGps_blank_mask,
      EMbmWficonsGps_no_signal_mask,
      EMbmWficonsGps_useless_mask,
      EMbmWficonsGps_poor_mask,
      EMbmWficonsGps_descent_mask,
      EMbmWficonsGps_good_mask,
      EMbmWficonsDemo1_speed_4_mask,
      EMbmWficonsDemo1_speed_2_mask,
      EMbmWficonsDemo1_speed_1_mask,
      EMbmWficonsDemo1_speed_h_mask,
   };
#else
   TInt GpsToMbmId[] = {
      EMbmWficonsGps1,
      EMbmWficonsGps2,
      EMbmWficonsGps1,
      EMbmWficonsGps1,
      EMbmWficonsGps1,
      EMbmWficonsGps1,
      EMbmWficonsDemo1_speed_h,
      EMbmWficonsDemo1_speed_1,
      EMbmWficonsDemo1_speed_2,
      EMbmWficonsDemo1_speed_4,
      EMbmWficonsGps1_m_no_gps,
      EMbmWficonsGps2_m,
      EMbmWficonsGps1_m_useless_gps,
      EMbmWficonsGps1_m_poor_gps,
      EMbmWficonsGps1_m_decent_gps,
      EMbmWficonsGps1_m_good_gps,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
      EMbmWficonsDemo1_mask,
   };
#endif
   if (!iGpsQualityFrames) {
      /* Need to create array. */
      iGpsQualityFrames = new CAnimatorFrame*[numGpsQualityFrames];
      for (int i = 0; i < numGpsQualityFrames; i++) {
         iGpsQualityFrames[i] = NULL;
      }
   }
   if (!iGpsQualityFrames[index]) {
      /* Need to load bitmaps for this frame. */
      CAnimatorFrame *frame = new (ELeave) CAnimatorFrame();
      CleanupStack::PushL(frame);
      frame->LoadBitmapAndMaskL(iMbmPath, 
                                GpsToMbmId[index], 
                                GpsToMbmId[index+numGpsQualityFrames], 
                                WFLayoutUtils::CalculateSizeUsingContextPane(GPS_FRAME_SIZE));
      iGpsQualityFrames[index] = frame;
      CleanupStack::Pop(frame);
   }
   return iGpsQualityFrames[index];
}

CFbsBitmap *
CContextPaneAnimator::MergeToContextPaneL(CFbsBitmap *anim, CFbsBitmap *gps)
{
   if (!anim || !gps) {
      /* "Input error". We need both bitmaps to be non-NULL. */
      return NULL;
   }
   /* Create the bitmap into which we will copy the component images. */
   CFbsBitmap* offScreenBitmap = new (ELeave) CFbsBitmap();
   /* "Push it". */
   CleanupStack::PushL(offScreenBitmap);
   /* "Initialize it" to known values. */
   // Note: made a define for the size at the top of the file,
   //       it will be easier to do changes since v3 is depending
   //       on the sizes and positions for icon in v2.
   User::LeaveIfError(offScreenBitmap->
                      Create(TSize(WFLayoutUtils::CalculateXValue(GPS_WIDTH), 
                                   anim->SizeInPixels().iHeight + gps->SizeInPixels().iHeight),
#ifndef NAV2_CLIENT_SERIES60_V3
                             EColor256)); 
#else
                             anim->DisplayMode()));
#endif
   /* Create the bitmap device. */
   CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(offScreenBitmap);
   /* "Push it". */
   CleanupStack::PushL(bitmapDevice);

   /* Create context pointer. */
   CFbsBitGc* bitmapContext=NULL;
   /* Send context pointer to bitmap device so it can create the context. */
   User::LeaveIfError(bitmapDevice->CreateContext(bitmapContext));
   /* "Push it". */
   CleanupStack::PushL(bitmapContext);

   /* Copy component image 1 to context (and thus to the offScreenBitmap) */
   bitmapContext->BitBlt(TPoint(0,0), anim);
   bitmapContext->BitBlt(TPoint(0, anim->SizeInPixels().iHeight), gps);
   /* Pop goes the context. */
   /* Pop goes the bitmap device. */
   CleanupStack::PopAndDestroy(2, bitmapDevice);

   /* Pop the offScreenBitmap from CleanupStack, but don't delete it. */
   /* It's our return value. */
   CleanupStack::Pop(offScreenBitmap);
   return offScreenBitmap;
}

CAnimatorFrame *
CContextPaneAnimator::MergeToContextPaneL(CAnimatorFrame* anim, CAnimatorFrame* gps)
{
   if (!anim || !gps) {
      /* "Input error". We need both frames to be non-NULL. */
      return NULL;
   }
   /* Merge image. */
   CFbsBitmap *img = MergeToContextPaneL(anim->GetBitmap(), gps->GetBitmap());
   /* Push it in case the MergeToContextPaneL for the mask fails. */
   CleanupStack::PushL(img);
   /* Merge mask. */
   CFbsBitmap *img_m = MergeToContextPaneL(anim->GetMask(), gps->GetMask());
   /* Mask creation worked, pop the img. */
   CleanupStack::Pop(img);

   /* Create return value and initialize it to the new merged images. */
   CAnimatorFrame *tmp = new CAnimatorFrame();
   tmp->SetBitmap(img);
   tmp->SetMask(img_m);
   return tmp;
}

CAnimatorFrame *
CContextPaneAnimator::GetCurrentFrameL()
{
   if (!iCurrentOk) {
      /* Remove old animation frame. */
      delete iCurrentFrame;
      iCurrentFrame = NULL;

      /* Create new animation pane. */

      /* Get component images. */
      CAnimatorFrame *anim = GetAnimationFrameL(iCurrentFrameIndex);
      CAnimatorFrame *gps  = GetGpsFrameL(iCurrentGpsStatus);

      /* Create new animation pane. */
      iCurrentFrame = MergeToContextPaneL(anim, gps);
      if (iCurrentFrame) {
         iCurrentOk = ETrue;
      }
   }
   CAnimatorFrame *tmpFrame = new CAnimatorFrame();
   CleanupStack::PushL(tmpFrame);

   tmpFrame->CopyFrameL(iCurrentFrame);
   CleanupStack::Pop(tmpFrame);
   return tmpFrame;
}
TInt 
CContextPaneAnimator::IncCurrentFrame(TInt /*count*/)
{
   TInt oldIdx = iCurrentFrameIndex;
   iCurrentFrameIndex++;
   if (iCurrentFrameIndex >= numAnimationFrames) {
      /* Wrap around. */
      iCurrentFrameIndex = 0;
   }
   iCurrentOk = EFalse;
   return oldIdx;
}
TInt 
CContextPaneAnimator::DecCurrentFrame(TInt /*count*/)
{
   TInt oldIdx = iCurrentFrameIndex;
   iCurrentFrameIndex--;
   if (iCurrentFrameIndex < 0) {
      /* Wrap around. */
      iCurrentFrameIndex = numAnimationFrames-1;
   }
   iCurrentOk = EFalse;
   return oldIdx;
}
TInt 
CContextPaneAnimator::SetCurrentFrame(TInt newFrame)
{
   TInt oldIdx = iCurrentFrameIndex;
   if (newFrame >= 0 && newFrame < numAnimationFrames) {
      iCurrentFrameIndex = newFrame;
      iCurrentOk = EFalse;
   }
   return oldIdx;
}
TInt
CContextPaneAnimator::SetCurrentGpsStatus(TInt newQuality)
{
   TInt oldQuality = iCurrentGpsStatus;
   if (iCurrentGpsStatus != newQuality) {
      iCurrentGpsStatus = newQuality;
      iCurrentOk = EFalse;
   }
   return oldQuality;
}

void
CContextPaneAnimator::CalculateSize(const TRect& aRect,
                                    TSize& aNewSize, 
                                    const TSize aOldSize,
                                    TInt aSceenWidth, 
                                    TInt aScreenHeight)
{
   aNewSize.SetSize(((TInt)((float)aRect.Width() * 
                            ((float)aOldSize.iWidth / aSceenWidth))), 
                    ((TInt)((float)aRect.Height() * 
                            ((float)aOldSize.iHeight / aScreenHeight))));   
}

