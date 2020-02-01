import { ImageLoaded, StartImageProcessing, GotProcessingResult, GotProcessingError, SetProcessingType } from "../constants/action";
import { ImageLoadedAction, SetProcessingTypeAction } from "../types/action";
import { RcFile } from "antd/lib/upload";
export const imageLoaded = (image: RcFile): ImageLoadedAction => {
    return {
        type: ImageLoaded,
        payload: image
    }
}

export const setImageProcessingType = (type: number): SetProcessingTypeAction =>{
    return {
        type: SetProcessingType,
        payload: type
    }
}

