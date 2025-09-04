import java.util.Scanner;

public class rollNo22_subnet {

    // Method to convert a byte to 8-bit binary string
    private static String toBinaryString(int num) {
        return String.format("%8s", Integer.toBinaryString(num)).replace(' ', '0');
    }

    // Method to convert dotted-decimal IP to binary
    private static String ipToBinary(String ip) {
        String[] parts = ip.split("\\.");
        StringBuilder binaryIP = new StringBuilder();
        for (String part : parts) {
            binaryIP.append(toBinaryString(Integer.parseInt(part))).append(" ");
        }
        return binaryIP.toString().trim();
    }

    // Calculate new CIDR prefix length
    private static int calculateNewPrefix(int originalPrefix, int requiredSubnets) {
        int bitsNeeded = (int) Math.ceil(Math.log(requiredSubnets) / Math.log(2));
        return originalPrefix + bitsNeeded;
    }

    // Convert CIDR prefix to dotted decimal subnet mask
    private static String cidrToSubnetMask(int cidr) {
        int mask = 0xffffffff << (32 - cidr);
        int octet1 = (mask >>> 24) & 0xff;
        int octet2 = (mask >>> 16) & 0xff;
        int octet3 = (mask >>> 8) & 0xff;
        int octet4 = mask & 0xff;
        return octet1 + "." + octet2 + "." + octet3 + "." + octet4;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Input IP Address
        System.out.print("Enter IP address (e.g., 192.168.1.0): ");
        String ipAddress = scanner.nextLine();

        // Input CIDR prefix (e.g., 24 for /24)
        System.out.print("Enter network mask (CIDR format, e.g., 24): ");
        int cidr = scanner.nextInt();

        // Display binary forms
        System.out.println("\n--- Binary Representation ---");
        System.out.println("IP Address (Binary):     " + ipToBinary(ipAddress));
        System.out.println("Subnet Mask (Binary):    " + ipToBinary(cidrToSubnetMask(cidr)));

        // Input number of required subnets
        System.out.print("\nEnter number of subnets required: ");
        int requiredSubnets = scanner.nextInt();

        // Calculate new CIDR and Subnet Mask
        int newCIDR = calculateNewPrefix(cidr, requiredSubnets);
        String newSubnetMask = cidrToSubnetMask(newCIDR);

        // Display result
        System.out.println("\n--- Subnetting Result ---");
        System.out.println("New CIDR Notation: /" + newCIDR);
        System.out.println("New Subnet Mask:   " + newSubnetMask);
        System.out.println("Subnet Mask (Binary): " + ipToBinary(newSubnetMask));

        scanner.close();
    }
}
