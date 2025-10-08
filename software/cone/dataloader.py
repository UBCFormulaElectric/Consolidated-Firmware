import json
from pathlib import Path

import matplotlib.patches as patches
import matplotlib.pyplot as plt
import numpy as np
import torch
from PIL import Image, ImageDraw, ImageFont
from torch.utils.data import DataLoader, Dataset


class FSOCOJsonDataset(Dataset):
    def __init__(
        self,
        image_dir: Path,
        json_dir: Path,
        class_map=None,
        transforms=None,
        border=140,
    ):
        self.image_dir = Path(image_dir)
        self.json_dir = Path(json_dir)
        self.border = border
        self.image_files = sorted(
            [
                f
                for f in self.image_dir.iterdir()
                if f.suffix.lower() in [".jpg", ".jpeg", ".png"]
            ]
        )
        self.transforms = transforms

        # Build class mapping if not provided
        if class_map is None:
            class_map = {}
            for img_path in self.image_files:
                json_path = self.json_dir / f"{img_path.name}.json"  # ← fixed
                if json_path.exists():
                    with open(json_path, "r") as jf:
                        data = json.load(jf)
                        for obj in data.get("objects", []):
                            title = obj["classTitle"]
                            if title not in class_map:
                                class_map[title] = len(class_map) + 1
        self.class_map = class_map

    def __len__(self):
        return len(self.image_files)

    def _crop_border(self, image: Image.Image) -> Image.Image:
        """Remove black watermark borders (140px each side)."""
        w, h = image.size
        b = self.border
        return image.crop((b, b, w - b, h - b))

    def __getitem__(self, idx):
        img_path = self.image_files[idx]
        json_path = self.json_dir / f"{img_path.name}.json"

        image = Image.open(img_path).convert("RGB")
        w, h = image.size

        # Remove watermark borders
        image = self._crop_border(image)

        if not json_path.exists():
            return image, {"boxes": torch.empty((0, 4)), "labels": torch.tensor([])}

        with open(json_path, "r") as jf:
            data = json.load(jf)

        boxes, labels = [], []
        b = self.border

        for obj in data.get("objects", []):
            if obj.get("geometryType") != "rectangle":
                continue
            pts = obj["points"]["exterior"]
            x_min, y_min = pts[0]
            x_max, y_max = pts[1]

            # Adjust coordinates after cropping
            x_min -= b
            x_max -= b
            y_min -= b
            y_max -= b

            # Skip boxes that fall outside the cropped area
            if x_max <= 0 or y_max <= 0:
                continue

            boxes.append([x_min, y_min, x_max, y_max])
            labels.append(self.class_map.get(obj["classTitle"], -1))

        boxes = torch.tensor(boxes, dtype=torch.float32)
        labels = torch.tensor(labels, dtype=torch.int64)
        target = {"boxes": boxes, "labels": labels, "image_id": torch.tensor([idx])}

        if self.transforms:
            image = self.transforms(image)

        return image, target

    def render(self, image, target, figsize=(12, 8), alpha=0.5):
        """
        Render image with bounding boxes and labels with better visualization.

        Args:
            image: torch.Tensor (C,H,W) or PIL.Image
            target: dict with keys 'boxes' (Nx4 tensor) and 'labels' (Nx1 tensor)
            figsize: tuple, size of matplotlib figure
            alpha: float, transparency of bounding boxes
        """
        # Convert image to numpy
        if isinstance(image, torch.Tensor):
            image = image.permute(1, 2, 0).cpu().numpy()
        elif isinstance(image, Image.Image):
            image = np.array(image)

        if image.dtype != np.uint8:
            image = (image * 255).astype(np.uint8)

        fig, ax = plt.subplots(1, 1, figsize=figsize)
        ax.imshow(image)

        boxes = target["boxes"].cpu().numpy()
        labels = target["labels"].cpu().numpy()

        # Assign a unique color per class
        colors = plt.colormaps["tab20"].resampled(len(self.class_map))

        for box, label in zip(boxes, labels):
            x_min, y_min, x_max, y_max = box
            class_name = [k for k, v in self.class_map.items() if v == label]
            class_name = class_name[0] if class_name else str(label)
            color = colors(label % 20)  # cycle through colors

            # Draw rectangle with alpha
            rect = patches.Rectangle(
                (x_min, y_min),
                x_max - x_min,
                y_max - y_min,
                linewidth=2,
                edgecolor=color,
                facecolor="none",
                alpha=alpha,
            )
            ax.add_patch(rect)

            # Draw label with filled background
            ax.text(
                x_min,
                max(y_min - 5, 0),  # ensure text is not clipped
                class_name,
                color="white",
                fontsize=max(int(image.shape[0] / 100), 10),
                bbox=dict(facecolor=color, alpha=0.7, pad=2, edgecolor="none"),
            )

        ax.axis("off")
        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    from torchvision import transforms as T

    json_dir = Path(
        "C:/Users/e1831/ubc-formula/Consolidated-Firmware/software/cone/data/fsoco_bounding_boxes_train/ampera/ann"
    )
    image_dir = Path(
        "C:/Users/e1831/ubc-formula/Consolidated-Firmware/software/cone/data/fsoco_bounding_boxes_train/ampera/img"
    )

    transform = T.Compose([T.ToTensor()])
    dataset = FSOCOJsonDataset(image_dir, json_dir, transforms=transform, border=140)

    for i in range(len(dataset)):
        img, target = dataset[i]
        dataset.render(img, target)
